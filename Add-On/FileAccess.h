/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAPROJECTFA_H
#define LAVAPROJECTFA_H

#include <Directory.h>
#include <File.h>
#include <String.h>
#include <List.h>
#include <VolumeRoster.h>
#include <stdio.h>
#include <dirent.h>
#include <Bitmap.h>
#include <Node.h>
#include <NodeInfo.h>
#include <Mime.h>
#include <Query.h>
#include <VolumeRoster.h>
#include <Path.h>

#include "FileTree.h"
#include "Exceptions/LavaMainException.h"

class FileAccess {
	public:
		FileAccess();
		~FileAccess();
		BList *getFiles();
		off_t getFileSize();
		BList* getTypes();
		FileTree *getFileTree();
		BBitmap* getIconOfFile(BString strPath);
		void setStateContainer(BString *StateCont);
		void setFiles(BList *FileList);
		void generateFileInfos();
	
	private:
		BDirectory *fDirectory;
		BFile *fFileAcces;
		BNode *fNode;
		BQuery *fQuery;
		BString *fFileName;
		BString *fPathAndFile;
		FileTree *fObjFileTree;
		off_t _walkThroughFolder(BString *Path);
		bool _FileTypeInTypeList(BString strType);
		void _addToSizeOfFiles(off_t Size);
		bool fIsGenerated;
		char buffer[255];
		BVolumeRoster *fVolumes;
		BVolume *fVolume;
		BEntry *fEntry;
		BPath *fPath;
		BString *fStateContainer;
	
	protected:
		void walkThroughFiles();
		bool getFilesFromQuery(BString strQueryFile, BString strParent, off_t *sizeOfFolder);
		void getTypeOfFilesByAttr();
		BList *FileList;
		BList *TypeList;
		off_t intSizeOfFiles;
};

#endif
