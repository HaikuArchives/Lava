/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/
#include <List.h>
#include <String.h>
#include <Entry.h>
#include <MediaFile.h>
#include <MediaTrack.h>
#include <MediaFormats.h>
#include <stdlib.h>

class Audio2wave {
	public:
		Audio2wave();
		~Audio2wave();
		void setAudioFiles(BList *lstPath);
		BList *convert2wave();
		status_t ConvertFile(BMediaFile *inFile, BMediaFile *outFile,
							   media_codec_info *audioCodec,
							   media_codec_info *videoCodec);
		
	private:
	
	protected:
		BList *lstPath2Audio;
		BList *lstWaveFiles;
};
