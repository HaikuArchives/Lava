/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/
#include "audio2wave.h"
#include <stdio.h>

Audio2wave::Audio2wave()
{
}

Audio2wave::~Audio2wave()
{
}

void Audio2wave::setAudioFiles(BList *lstPath)
{
	lstPath2Audio = lstPath;
}

BList *Audio2wave::convert2wave()
{	
	lstWaveFiles = new BList();
	entry_ref ref;
	BMediaFile *objMediaFile;
	BMediaTrack *objMediaTrack;
	media_codec_info *audioCodec;
	media_format format;
		
	for(int i = 0; i < lstPath2Audio->CountItems(); i++)
	{
		get_ref_for_path(((BString*)lstPath2Audio->ItemAt(i))->String(), &ref);
		objMediaFile = new BMediaFile(&ref);
		
		for(int j = 0; j < objMediaFile->CountTracks(); j++)
		{
			objMediaTrack = objMediaFile->TrackAt(i);
			format.type = B_MEDIA_RAW_AUDIO;
			
			ConvertFile(objMediaFile, objMediaFile, audioCodec, NULL);
		}
	}
	
	return lstWaveFiles;
}

status_t 
Audio2wave::ConvertFile(BMediaFile *inFile, BMediaFile *outFile,
							   media_codec_info *audioCodec,
							   media_codec_info *videoCodec)
{
	BMediaTrack *inVidTrack(NULL), *inAudTrack(NULL);
	BMediaTrack *outVidTrack(NULL), *outAudTrack(NULL);
	media_format inFormat, outAudFormat, outVidFormat;
	media_raw_audio_format *raf(NULL);
	media_raw_video_format *rvf(NULL);
	int32 width(-1), height(-1);
	short audioFrameSize(1);
	char *videoData(NULL);
	char *audioData(NULL);

	// gather the necessary format information and construct output tracks
	int32 tracks = inFile->CountTracks();
	for (int32 i = 0; i < tracks && (!outAudTrack || !outVidTrack); i++)
	{
		BMediaTrack *inTrack = inFile->TrackAt(i);
		inTrack->EncodedFormat(&inFormat);

		if (inFormat.IsAudio() && (audioCodec != NULL))
		{
			inAudTrack = inTrack;
			memcpy(&outAudFormat, &inFormat, sizeof(outAudFormat));
			outAudFormat.type = B_MEDIA_RAW_AUDIO;
			raf = &(outAudFormat.u.raw_audio);
			inTrack->DecodedFormat(&outAudFormat);
			
			audioData = (char*)malloc(raf->buffer_size);
			audioFrameSize = (raf->format & 0xf)
								* raf->channel_count;
			outAudTrack = outFile->CreateTrack(&outAudFormat, audioCodec);	
		}
		else if (inFormat.IsVideo() && (videoCodec != NULL))
		{
			inVidTrack = inTrack;
			width = (int32)inFormat.Width();
			height = (int32)inFormat.Height();
			
			// construct desired decoded video format
			memset(&outVidFormat, 0, sizeof(outVidFormat));
			outVidFormat.type = B_MEDIA_RAW_VIDEO;
			rvf = &(outVidFormat.u.raw_video);
			rvf->last_active = (uint32)(height - 1);
			rvf->orientation = B_VIDEO_TOP_LEFT_RIGHT;
			rvf->pixel_width_aspect = 1;
			rvf->pixel_height_aspect = 3;
			rvf->display.format = B_RGB32;
			rvf->display.bytes_per_row = 4 * width;
			rvf->display.line_width = width;
			rvf->display.line_count = height;

			inVidTrack->DecodedFormat(&outVidFormat);
			videoData = (char *)malloc(width * height * 4);
			outVidTrack = outFile->CreateTrack(&outVidFormat, videoCodec);
		}
		else
		{
			inFile->ReleaseTrack(inTrack);		
		}
	}

	int64 frameCount, framesRead;
	status_t err;
	media_header mh;
	int32 lastPercent, currPercent;
	float completePercent;
	BString status;

	// read video from source and write to destination, if necessary
	if (outVidTrack != NULL)
	{
		frameCount = inVidTrack->CountFrames();
		lastPercent = -1;
		for (int64 i = 0; (i < frameCount); i++)
		{
			framesRead = 1;
			if ((err = inVidTrack->ReadFrames(videoData, &framesRead, &mh)) != B_OK)
			{
				fprintf(stderr, "Error reading video frame %Ld: %s\n", i,
						strerror(err));
				break;
			}
			
			if ((err = outVidTrack->WriteFrames(videoData, 1, mh.u.encoded_video.field_flags)) != B_OK)
			{
				fprintf(stderr, "Error writing video frame %Ld: %s\n", i,
						strerror(err));
				break;
			}
		}
		outVidTrack->Flush();
		inFile->ReleaseTrack(inVidTrack);
	}
	
	// read audio from source and write to destination, if necessary
	if (outAudTrack != NULL)
	{
		frameCount = inAudTrack->CountFrames();
		lastPercent = -1;
		for (int64 i = 0; (i < frameCount); i += framesRead)
		{
			if ((err = inAudTrack->ReadFrames(audioData, &framesRead, &mh)) != B_OK)
			{
				fprintf(stderr, "Error reading audio frames: %s\n", strerror(err));
				break;
			}
			
			if ((err = outAudTrack->WriteFrames(audioData, framesRead)) != B_OK)
			{
				fprintf(stderr, "Error writing audio frames: %s\n", strerror(err));
				break;
			}
			
			completePercent = ((float)i) / ((float)frameCount) * 100;
			currPercent = (int16)floor(completePercent);
		}
		
		outAudTrack->Flush();
		inFile->ReleaseTrack(inAudTrack);
	}

	outFile->CloseFile();
	delete outFile;
	free(videoData);
	free(audioData);
	
	return B_OK;
}
