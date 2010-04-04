/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#include "ProjectTypeSelector.h"

ProjectTypeSelector::ProjectTypeSelector()
: intPlayTime(0), objFileAccess(NULL)
{
	 CDSIZE = 734003200;
	 AUDIOCDPLAYTIME = 4800;
}


ProjectTypeSelector::~ProjectTypeSelector()
{
}


void
ProjectTypeSelector::setFileAccess(FileAccess *objFileAccess)
{
	this->objFileAccess = objFileAccess;
}


/*	isVolume
*	checks if selected file is a volume
*	if yes, returns true
*/	
bool
ProjectTypeSelector::isVolume()
{
	BString *SelectedFile = (BString*)FileList->FirstItem();
	BString SelectedFileTMP;
	BVolumeRoster objVolumeRoster;
	objVolumeRoster.Rewind();
	BVolume objVolume;
	char chVolumeName[B_FILE_NAME_LENGTH];
	
	SelectedFile->CopyInto(SelectedFileTMP, 0, (int)SelectedFile->Length());	
	SelectedFileTMP.RemoveAll("/");
	while(objVolumeRoster.GetNextVolume(&objVolume) != B_BAD_VALUE) {
		objVolume.GetName(chVolumeName);
		
		if(SelectedFileTMP.Compare(chVolumeName) == 0 && objVolume.IsReadOnly()) {
			fSizeOfFiles = objVolume.Capacity();
			return true;
		}
	}
	
	return false;
}


/*	isCue
*	checks if selected file is a cue file (checks by file extension)
*	if yes, returns true
*/
bool
ProjectTypeSelector::isCue()
{
	BString *SelectedFile = (BString*)FileList->FirstItem();
	BString SelectedFileTMP;
	BString FileExtension;
	BString tmp;
	
	//get file extension
	SelectedFile->CopyInto(SelectedFileTMP, 0, (int)SelectedFile->Length());
	SelectedFileTMP.RemoveAll("/");
	for(int i = 3; i > 0; i--) {
		tmp.SetTo(SelectedFileTMP.ByteAt((SelectedFileTMP.Length() - i)), 1);
		FileExtension.Insert(tmp.String(), FileExtension.Length());
	}
	
	FileExtension.ToUpper();
	if(FileExtension.Compare("CUE") == 0)
		return true;
	
	return false;
}


/*	isImage
*	checks if selected file is a image
*	if yes, returns true
*/
bool
ProjectTypeSelector::isImage()
{
	BString *FileType = (BString*)TypeList->FirstItem();

	if(FileType->Compare("application/octet-stream") == 0 ||
		FileType->Compare("application/x-cd-image") == 0)
		return true;
	
	return false;
}


/*	isAudioDisc
*	checks if selected files are audio files, and greps the playtime of them
*	when all files audio files
*	returns true if audio disc
*/
bool
ProjectTypeSelector::isAudioDisc()
{
	BString *FileType;
	
	for(int i = 0; i < TypeList->CountItems(); i++) {
		FileType = (BString*)TypeList->ItemAt(i);
		if(FileType->Compare("audio", 5) != 0)
			return false;
	}
	
	entry_ref ref;
	BMediaFile *mediaFile;
	bigtime_t time = 0;
	
	for(int i = 0; i < FileList->CountItems(); i++){
		get_ref_for_path(((BString*)FileList->ItemAt(i))->String(), &ref);
		mediaFile = new BMediaFile(&ref);
		BMediaTrack *track = NULL;
		
		for(int j = 0; j < mediaFile->CountTracks(); j++){
			track = mediaFile->TrackAt(j);
			time = track->Duration();
			intPlayTime += ((int)time / 1000000);
		}
		
		delete mediaFile;
	}
		
	return true;
}


/*	isVideoDisc
*	checks if selected files are video files
*	if yes, returns true
*/
bool
ProjectTypeSelector::isVideoDisc()
{
	BString *FileType;

	for(int i = 0; i < TypeList->CountItems(); i++) {
		FileType = (BString*)TypeList->ItemAt(i);
		if(FileType->Compare("video", 5) != 0)
			return false;
	}

	return true;
}


/*	isProject
* checks if an selected folder is a lava project folder.
*/
bool
ProjectTypeSelector::isProject()
{
	BNode objNode;
	BString *Project = (BString*)FileList->FirstItem();
	objNode.SetTo(Project->String());
	
	char buffer[500];
	memset(buffer, 0, sizeof(buffer));

	if(objNode.ReadAttr("LAVA:Type", B_STRING_TYPE, 0, buffer, sizeof(buffer)) == B_ENTRY_NOT_FOUND)
		return false;
	else {
		BDirectory objDir;
		objDir.SetTo(Project->String());
		if(objDir.InitCheck() != B_OK)
			return false;
		else
			return true;
	}
}


/*	getDiscType
*	method checks which kind of cd/dvd is to burn
*	returns integer which is defined in header (AI.h)
*/
int
ProjectTypeSelector::getDiscType()
{		
	if(objFileAccess == NULL)
		throw new ProjectTypeSelectorException(new BString("FileAccess object isn't seted ProjectTypeSelector::getDiscType()"));

	objFileAccess->generateFileInfos();
	TypeList = objFileAccess->getTypes();
	fSizeOfFiles = objFileAccess->getFileSize();
	FileList = objFileAccess->getFiles();
	
	if(FileList->CountItems() == 0) {
		return EMPTYLIST;
	}
	
	if(FileList->CountItems() == 1 && isVolume()) {
	
		//CD or DVD
		if(fSizeOfFiles > CDSIZE) {
			return DVDCOPY;
		}
		else {
			return CDCOPY;
		}
	}
	else if(FileList->CountItems() == 1 && isCue()) {
		return CUE;
	}
	else if(FileList->CountItems() == 1 && isImage()) {
		
		//CD or DVD
		if(fSizeOfFiles > CDSIZE) {
			return DVDIMAGE;
		}
		else {
			return CDIMAGE;
		}
	}
	else if(FileList->CountItems() == 1 && isProject()) {
		return PORJECT;
	}
	
	if(isAudioDisc() && intPlayTime > AUDIOCDPLAYTIME) {
		return AUDIODVD;
	}
	else if(isAudioDisc()) {
		return AUDIOCD;
	}
		
	//CD or DVD
	if(fSizeOfFiles > CDSIZE) {
		return DATADVD;
	}
	else {
		
		//Video CD or data CD
		if(isVideoDisc()) {
			return VCD;
		}
		else {
			return DATACD;
		}
	}
	
	return ERROR;
}
