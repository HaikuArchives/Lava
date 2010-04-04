/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#include "FileAccess.h"
#include <Alert.h>

FileAccess::FileAccess()
: intSizeOfFiles(0), fStateContainer(NULL)
{
	fIsGenerated = false;
	FileList = new BList();
	TypeList = new BList();
	fVolumes = new BVolumeRoster();
}


FileAccess::~FileAccess()
{
	delete TypeList, FileList, fVolumes;
}


void
FileAccess::setStateContainer(BString *StateCont)
{
	fStateContainer = StateCont;
}


void
FileAccess::setFiles(BList *FileList)
{
	this->FileList = FileList;
	fIsGenerated = false;
}


BList*
FileAccess::getFiles()
{
	return this->FileList;
}

BList*
FileAccess::getTypes()
{
	return this->TypeList;
}


off_t
FileAccess::getFileSize()
{
	return intSizeOfFiles;
}


/*	getFileTree
*	returns the files as tree
*/
FileTree*
FileAccess::getFileTree()
{
	return fObjFileTree;
}


/*	getIconOfFile
*	returns icons as Bitmap
*/
BBitmap*
FileAccess::getIconOfFile(BString strPath)
{
	BNode *node = new BNode(strPath.String());
	BNodeInfo *info = new BNodeInfo();
	info->SetTo(node);
	
	#if __HAIKU__
		BBitmap *Icon = new BBitmap(BRect(0, 0, B_MINI_ICON - 1, B_MINI_ICON - 1), B_RGBA32); //for haiku
	#else
		BBitmap *Icon = new BBitmap(BRect(0, 0, B_MINI_ICON - 1, B_MINI_ICON - 1), B_COLOR_8_BIT); //BeOS, Zeta, etc.
	#endif
	
	info->GetTrackerIcon(Icon, B_MINI_ICON);        
	delete node, info;

	return Icon;
}


/*	FileTypeInTypeList
*	checks if a filetype is already in the type list
*/
bool
FileAccess::_FileTypeInTypeList(BString strType)
{
	BString *strTMP;

	for(int i = 0; i < TypeList->CountItems(); i++) {
		strTMP = (BString*)TypeList->ItemAt(i);
		
		if(strTMP->Compare(strType) == 0)
			return true;
	}
	
	return false;
}


/*	getTypeOfFilesByAttr
*	method read the type of a list of files and
*	and write it to a BList object ((protected)TypeList)
*/
void
FileAccess::getTypeOfFilesByAttr()
{
	BString *strTMP;
	BString strTMP2;
	BNode *node = new BNode();
		
	for(int i = 0; i < FileList->CountItems(); i++) {
		strTMP = (BString*)FileList->ItemAt(i);
		node = new BNode(strTMP->String());	
		memset(buffer, 0, sizeof(buffer));
		node->ReadAttr("BEOS:TYPE", B_STRING_TYPE, 0, buffer, sizeof(buffer));
		
		if(!_FileTypeInTypeList(buffer))
			TypeList->AddItem(new BString(buffer));
			
		delete node;
	}
}


/*	_addToSizeOfFiles
*	method sum the size of files
*/
void
FileAccess::_addToSizeOfFiles(off_t Size)
{
	intSizeOfFiles += Size;
}


/*	getFilesFromQuery
*	method get files from a query and put them to filestructure
*/
bool
FileAccess::getFilesFromQuery(BString strQueryFile, BString strParent, off_t *sizeOfFolder)
{
	fNode->SetTo(strQueryFile.String());
	memset(buffer, 0, sizeof(buffer));
	fNode->ReadAttr("BEOS:TYPE", B_STRING_TYPE, 0, buffer, sizeof(buffer));

	if(strcmp(buffer, "application/x-vnd.Be-query") == 0) {
		off_t TMP = 0;
		off_t *ptFolderSize;
		
		fQuery->Clear();
		memset(buffer, 0, sizeof(buffer));
		fNode->ReadAttr("_trk/qrystr", B_STRING_TYPE, 0, buffer, sizeof(buffer));
		fQuery->SetPredicate(buffer);
		
		while(fVolumes->GetNextVolume(fVolume) != B_BAD_VALUE) {
			fQuery->SetVolume(fVolume);
			fQuery->Fetch();
			
			while(fQuery->GetNextEntry(fEntry) == B_OK) {
				fEntry->GetPath(fPath);
				fDirectory->SetTo(fPath->Path());
				
				if(fDirectory->InitCheck() != B_OK) {
					if(strQueryFile.Compare(fPath->Path()) != 0) {
						fFileAcces->SetTo(fPath->Path(), B_READ_ONLY);
						fFileAcces->GetSize(&TMP);
						_addToSizeOfFiles(TMP);
						
						if(sizeOfFolder != NULL)
							sizeOfFolder += TMP;
						
						fObjFileTree->AddNode(strParent.String(), fPath->Path(), true, new off_t((int64)TMP), true);
					}
				}
				else {
					ptFolderSize = new off_t(0);
					fObjFileTree->AddNode(strParent.String(), fPath->Path(), false, ptFolderSize, true);
					*ptFolderSize = _walkThroughFolder(new BString(fPath->Path()));
				}
			}
		}
		fVolumes->Rewind();
		return true;
	}
	else
		return false;
}


/*	_walkThroughFolder
*	method walks through selected subfolders and gets size of them
*/
off_t
FileAccess::_walkThroughFolder(BString *Path)
{
	DIR *dir;// = new DIR();
	struct dirent *dirzeiger = new dirent;
	off_t TMP = 0;
	off_t *ptFolderSize;
	off_t sizeOfFolder = 0;
		
	if((dir = opendir(Path->String())) != NULL) {
		while((dirzeiger=readdir(dir)) != NULL) {
			if(strcmp((*dirzeiger).d_name, ".") != 0 && strcmp((*dirzeiger).d_name, "..") != 0) {					
				fFileName->SetTo((*dirzeiger).d_name);
				fPathAndFile->SetTo(Path->String());
				*fPathAndFile += "/";
				*fPathAndFile += *fFileName;
								
				fDirectory->SetTo(fPathAndFile->String());
				if(fDirectory->InitCheck() != B_OK) {
					if(!getFilesFromQuery(fPathAndFile->String(), Path->String(), &sizeOfFolder)) {
						fFileAcces->SetTo(fPathAndFile->String(), B_READ_ONLY);
						fFileAcces->GetSize(&TMP);
						_addToSizeOfFiles(TMP);
						sizeOfFolder += TMP;
						
						fStateContainer->SetTo(fFileName->String());
						
						fObjFileTree->AddNode(Path->String(), fPathAndFile->String(), true, new off_t((int64)TMP), false);
					}
				}
				else if(fDirectory->InitCheck() == B_OK) {
					ptFolderSize = new off_t(0);
					
					fStateContainer->SetTo(fFileName->String());
					
					fObjFileTree->AddNode(Path->String(), fPathAndFile->String(), false, ptFolderSize, false);
					*ptFolderSize = _walkThroughFolder(new BString(fPathAndFile->String()));
					sizeOfFolder += *ptFolderSize; 
				}
	   		}
    	}
    }
  	
	if(dir != NULL)
		closedir(dir);
		
	delete dirzeiger, Path;
	
	return sizeOfFolder;
}


/*	walkThroughFiles
*	method walks through selected files and build file tree and greps the size of
*	the selected files
*	and sume it to (protected)intSizeOfFiles
*/
void
FileAccess::walkThroughFiles()
{
	fObjFileTree = new FileTree();
	fDirectory = new BDirectory();
	fFileAcces = new BFile();
	
	fQuery = new BQuery();
	fNode = new BNode();
	fVolume = new BVolume();
	fEntry = new BEntry();
	fPath = new BPath();
	
	fPathAndFile = new BString();
	fFileName = new BString();	
	BString *strTMP;
	off_t TMP = 0;
	off_t *ptFolderSize;
	
	for(int i = 0; i < FileList->CountItems(); i++) {		
		strTMP = (BString*)FileList->ItemAt(i);	
		fDirectory->SetTo(strTMP->String());
		
		if(fDirectory->InitCheck() != B_OK) {			
			if(!getFilesFromQuery(strTMP->String(), "", NULL)) {
				fFileAcces->SetTo(strTMP->String(), B_READ_ONLY);
				fFileAcces->GetSize(&TMP);
				_addToSizeOfFiles(TMP);
				
				fStateContainer->SetTo(strTMP->String());
				
				fObjFileTree->AddNode("", strTMP->String(), true, new off_t((int64)TMP), false);
			}
		}
		else {
			ptFolderSize = new off_t(0);
			
			fStateContainer->SetTo(strTMP->String());
			
			fObjFileTree->AddNode("", strTMP->String(), false, ptFolderSize, false);
			*ptFolderSize = _walkThroughFolder(new BString(strTMP->String()));
		}
	}
	
	delete fDirectory, fFileAcces, fPathAndFile, fFileName, fNode, fQuery,
			fVolume, fVolumes, fEntry, fPath;
}


/*	generateFileInfos
*	this method generates all needed informations (size, attributes etc.)
*/
void
FileAccess::generateFileInfos()
{
	if(!fIsGenerated) {
		walkThroughFiles();
		getTypeOfFilesByAttr();
		
		fIsGenerated = true;
	}
}

