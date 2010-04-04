/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAPROJECTPTS_H
#define LAVAPROJECTPTS_H

#include <List.h>
#include <File.h>
#include <Directory.h>
#include <Path.h>
#include <SymLink.h>
#include <MediaDefs.h>
#include <KernelKit.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <MediaTrack.h>
#include <MediaFile.h>
#include <MediaFormats.h>

#include <String.h>
#include <stdio.h>
#include <dirent.h>

#include "FileAccess.h"
#include "Exceptions/ProjectTypeSelectorException.h"

#define CDIMAGE 0
#define DATACD 1
#define AUDIOCD 2
#define VCD 3
#define DATADVD 4
#define AUDIODVD 5
#define CUE 6
#define CDCOPY 7
#define DVDCOPY 8
#define EMPTYLIST 9
#define DVDIMAGE 10
#define PORJECT 11
#define ERROR -1

class ProjectTypeSelector {
	public:
		ProjectTypeSelector();
		~ProjectTypeSelector();
		void setFileAccess(FileAccess *objFileAccess);
		int getDiscType();
		
	private:
		off_t CDSIZE;
		off_t fSizeOfFiles;
		short int AUDIOCDPLAYTIME;
		bool _FileTypeInTypeList(BString strType);
		
	protected:
		void getTypeOfFilesByAttr();
		bool isVolume();
		bool isCue();
		bool isImage();
		bool isAudioDisc();
		bool isVideoDisc();
		bool isProject();
		BList *FileList;
		BList *TypeList;
		FileAccess *objFileAccess;
		off_t intPlayTime;
};

#endif
