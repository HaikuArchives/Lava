/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#include "LavaProjectManager.h"

LavaProjectManager::LavaProjectManager()
{
	strPathToProject = "";
	fLavaProject = NULL;
	fStateContainer = NULL;
	fProjectDiscType = -2;
}


LavaProjectManager::~LavaProjectManager()
{
}


void
LavaProjectManager::setStateContainer(BString *StateCont)
{
	fStateContainer = StateCont;
}


BString
LavaProjectManager::getActProjectPath()
{
	BString tmp = "";
	tmp += strPathToProject;
	tmp += fActProject;
	return tmp;
}


/*	CreateNewProject
*	creats a new lava project on hard drive
*/
void
LavaProjectManager::CreateNewProject(int DiscType)
{
	fObjLavaFilePanel = new LavaFilePanel();
	fObjLavaFilePanel->Run();
	fObjLavaFilePanel->Save();
	
	if(fObjLavaFilePanel->lfpPath == NULL || fObjLavaFilePanel->lfpProject == NULL)
		throw new ProjectManagerException(new BString("creating of project canceled (LavaProjectManager::CreateNewProject)"), fObjLavaFilePanel);
	
	strPathToProject = fObjLavaFilePanel->lfpPath->String();
	strPathToProject += "/";
	fObjDir = new BDirectory();
	fObjDir->SetTo(fObjLavaFilePanel->lfpPath->String());
	BString strTMP = "";
	
	if(fObjDir->CreateDirectory(fObjLavaFilePanel->lfpProject->String(), fObjDir) != B_OK)
		throw new ProjectManagerException(new BString("Creating project failed (LavaProjectManager::CreateNewProject)"), fObjLavaFilePanel, fObjDir);
	
	fNode = new BNode();
	strTMP = fObjLavaFilePanel->lfpPath->String();
	strTMP += "/";
	strTMP += fObjLavaFilePanel->lfpProject->String();
	fNode->SetTo(strTMP.String());
	
	if(fNode->WriteAttr("LAVA:PROJECTNAME", B_STRING_TYPE, 0, fObjLavaFilePanel->lfpProject->String(), fObjLavaFilePanel->lfpProject->Length()) == B_FILE_ERROR)
		throw new ProjectManagerException(new BString("can't write attributes to project (LavaProjectManager::CreateNewProject)"), fObjLavaFilePanel, fNode);
	
	if(fNode->WriteAttr("LAVA:Type", B_STRING_TYPE, 0, "rootfolder", sizeof("rootfolder")) == B_FILE_ERROR)
		throw new ProjectManagerException(new BString("can't write attributes to project (LavaProjectManager::CreateNewProject)"), fObjLavaFilePanel, fNode);

	if(fNode->WriteAttr("LAVA:PathToFolder", B_STRING_TYPE, 0, strTMP.String(), strTMP.Length()) == B_FILE_ERROR)
		throw new ProjectManagerException(new BString("can't write attributes to project (LavaProjectManager::CreateNewProject)"), fObjLavaFilePanel, fNode);

	if(fNode->WriteAttr("LAVA:DiscType", B_INT32_TYPE, 0, &DiscType, sizeof(DiscType)) == B_FILE_ERROR)
		throw new ProjectManagerException(new BString("can't write attributes to project (LavaProjectManager::CreateNewProject)"), fObjLavaFilePanel, fNode);
	
	fActProject.SetTo(fObjLavaFilePanel->lfpProject->String());
		
	fObjLavaFilePanel->Lock();
	fObjLavaFilePanel->Quit();
	delete fNode, fObjDir, fObjLavaFilePanel;
}


LavaProject* LavaProjectManager::OpenPorject(BString Porject)
{
	fNode = new BNode();		
	fFileName = new BString();
	fPathAndFile = new BString();
	fObjDir = new BDirectory();
	fObjFile = new BFile();
	objTempPath = new BPath();
	int DiscType;
	
	BDirectory objDir;
	objDir.SetTo(Porject.String());
	if(objDir.InitCheck() != B_OK)
		throw new ProjectManagerException(new BString("Not a lava project directory (LavaProjectManager::OpenPorject)"));
	
	fNode->SetTo(Porject.String());
	memset(buffer, 0, sizeof(buffer));
	if(fNode->ReadAttr("LAVA:Type", B_STRING_TYPE, 0, buffer, sizeof(buffer)) == B_ENTRY_NOT_FOUND)
		throw new ProjectManagerException(new BString("Not a project, subproject folder or project is damaged (LavaProjectManager::OpenPorject)"));
	
	if(fNode->ReadAttr("LAVA:DiscType", B_INT32_TYPE, 0, &DiscType, sizeof(DiscType)) == B_ENTRY_NOT_FOUND)
		throw new ProjectManagerException(new BString("can't read disc type of project (LavaProjectManager::OpenPorject)"), fObjLavaFilePanel, fNode);
	
	Porject.CopyInto(fActProject, Porject.FindLast("/") + 1, Porject.CountChars());
	objTempPath->SetTo(Porject.String());
	objTempPath->GetParent(objTempPath);
	strPathToProject = objTempPath->Path();
	strPathToProject += "/";
	
	fLavaProject = new LavaProject(fActProject.String());
	fLavaProject->DiscType = DiscType;
	fProjectDiscType = DiscType;
	_readPorject(new BString(Porject.String()));
	
	delete fNode, fFileName, fPathAndFile, fObjDir, fObjFile;
	
	return fLavaProject;
}


/*	OpenPorject
*	opens an excisting project and returns it
*/
LavaProject*
LavaProjectManager::OpenPorject()
{
	fObjLavaFilePanel = new LavaFilePanel();
	fObjLavaFilePanel->Run();
	fObjLavaFilePanel->Open();
	
	if(fObjLavaFilePanel->lfpPath == NULL)
		throw new ProjectManagerException(new BString("adding files to project canceled (LavaProjectManager::OpenPorject)"), fObjLavaFilePanel);
	
	fFileName = new BString();
	fPathAndFile = new BString();
	fObjDir = new BDirectory();
	fObjFile = new BFile();
	fNode = new BNode();
	objTempPath = new BPath();
	int DiscType;
	
	fObjLavaFilePanel->lfpPath->CopyInto(fActProject, fObjLavaFilePanel->lfpPath->FindLast("/") + 1, fObjLavaFilePanel->lfpPath->CountChars());
	objTempPath->SetTo(fObjLavaFilePanel->lfpPath->String());
	objTempPath->GetParent(objTempPath);
	strPathToProject = objTempPath->Path();
	strPathToProject += "/";
	
	fObjDir->SetTo(fObjLavaFilePanel->lfpPath->String());
	if(fObjDir->InitCheck() != B_OK)
		throw new ProjectManagerException(new BString("Not a lava project directory (LavaProjectManager::OpenPorject)"), fObjLavaFilePanel, fObjDir);
	
	fNode->SetTo(fObjLavaFilePanel->lfpPath->String());
	memset(buffer, 0, sizeof(buffer));
	if(fNode->ReadAttr("LAVA:Type", B_STRING_TYPE, 0, buffer, sizeof(buffer)) == B_ENTRY_NOT_FOUND)
		throw new ProjectManagerException(new BString("Not a project, subproject folder or project is damaged (LavaProjectManager::OpenPorject)"), fObjLavaFilePanel, fNode);
	
	if(fNode->ReadAttr("LAVA:DiscType", B_INT32_TYPE, 0, &DiscType, sizeof(DiscType)) == B_ENTRY_NOT_FOUND)
		throw new ProjectManagerException(new BString("can't read disc type of project (LavaProjectManager::OpenPorject)"), fObjLavaFilePanel, fNode);
	
	fLavaProject = new LavaProject(fActProject.String());
	fLavaProject->DiscType = DiscType;
	fProjectDiscType = DiscType;
	_readPorject(new BString(fObjLavaFilePanel->lfpPath->String()));
	
	fObjLavaFilePanel->Lock();
	fObjLavaFilePanel->Quit();
	
	delete objTempPath, fObjLavaFilePanel, fFileName, fPathAndFile, fObjDir,
			fObjFile, fNode;
	
	return fLavaProject;
}


/*	_readPorject
*	reads a project from hard drive and reads the informations
*	saved in the attributes (Casper-Wendy-Style)
*/
void
LavaProjectManager::_readPorject(BString *Path)
{
	DIR *dir;// = new DIR;
	struct dirent *dirzeiger = new dirent;
	int64 tmpSize = 0;
	bool Query = false;
	objTempPath = new BPath();
		
	if((dir = opendir(Path->String())) != NULL) {
		while((dirzeiger=readdir(dir)) != NULL) {
			if(strcmp((*dirzeiger).d_name, ".") != 0 && strcmp((*dirzeiger).d_name, "..") != 0) {					
				fFileName->SetTo((*dirzeiger).d_name);
				fPathAndFile->SetTo(Path->String());
				*fPathAndFile += "/";
				*fPathAndFile += *fFileName;
				
				memset(buffer, 0, sizeof(buffer));
				memset(buffer2, 0, sizeof(buffer2));
				
				fObjDir->SetTo(fPathAndFile->String());
				if(fObjDir->InitCheck() != B_OK) { //File
					fStateContainer->SetTo(fFileName->String());
					fNode->SetTo(fPathAndFile->String());
									
					if(fNode->ReadAttr("LAVA:PathToFile", B_STRING_TYPE, 0, buffer, sizeof(buffer)) == B_ENTRY_NOT_FOUND)
						throw new ProjectManagerException(new BString("an Attr. PathToFile doesn't exsist by some file (LavaProjectManager::_readPorject)"), fObjDir, fNode);
					
					if(fNode->ReadAttr("LAVA:Size", B_INT64_TYPE, 0, &tmpSize, 8) == B_ENTRY_NOT_FOUND)
						throw new ProjectManagerException(new BString("an Attr. Size doesn't exsist by some file"), fObjDir, fNode);
					
					if(fNode->ReadAttr("LAVA:Parent", B_STRING_TYPE, 0, buffer2, sizeof(buffer2)) == B_ENTRY_NOT_FOUND)
						throw new ProjectManagerException(new BString("an Attr. Parent doesn't exsist by some file (LavaProjectManager::_readPorject)"), fObjDir, fNode);

					if(fNode->ReadAttr("LAVA:Query", B_BOOL_TYPE, 0, &Query, sizeof(Query)) == B_ENTRY_NOT_FOUND)
						throw new ProjectManagerException(new BString("an Attr. query doesn't exsist by some file (LavaProjectManager::_readPorject)"), fObjDir, fNode);
					
					fLavaProject->ProjectSize += tmpSize;
					
					if(Query) {
						fEntry.SetTo(fPathAndFile->String());
						fEntry.GetParent(&fParentEntry);
						fParentEntry.GetPath(objTempPath);
						fNode->SetTo(objTempPath->Path());
						
						memset(buffer2, 0, sizeof(buffer2));
						if(fNode->ReadAttr("LAVA:PathToFolder", B_STRING_TYPE, 0, buffer2, sizeof(buffer2)) == B_ENTRY_NOT_FOUND)
							throw new ProjectManagerException(new BString("an Attr. PathToFolder doesn't exsist by some file (LavaProjectManager::_readPorject)"), fObjDir, fNode);
						
						int64 *tmp = new int64;
						*tmp = tmpSize;
						fLavaProject->ProjectStructure->AddNode(buffer2, buffer, true, tmp, true);
					}
					else {
						int64 *tmp = new int64;
						*tmp = tmpSize;
						fLavaProject->ProjectStructure->AddNode(buffer2, buffer, true, tmp, false);
					}
				}
				else if(fObjDir->InitCheck() == B_OK) { //Folder
					fStateContainer->SetTo(fFileName->String());
					fNode->SetTo(fPathAndFile->String());
					
					if(fNode->ReadAttr("LAVA:PathToFolder", B_STRING_TYPE, 0, buffer, sizeof(buffer)) == B_ENTRY_NOT_FOUND)
						throw new ProjectManagerException(new BString("an Attr. PathToFolder doesn't exsist by some folder (LavaProjectManager::_readPorject)"), fObjDir, fNode);
					
					if(fNode->ReadAttr("LAVA:Size", B_INT64_TYPE, 0, &tmpSize, 8) == B_ENTRY_NOT_FOUND)
						throw new ProjectManagerException(new BString("an Attr. Size doesn't exsist by some file"), fObjDir, fNode);
					
					if(fNode->ReadAttr("LAVA:Parent", B_STRING_TYPE, 0, buffer2, sizeof(buffer2)) == B_ENTRY_NOT_FOUND)
						throw new ProjectManagerException(new BString("an Attr. Parent doesn't exsist by some folder (LavaProjectManager::_readPorject)"), fObjDir, fNode);
						
					if(fNode->ReadAttr("LAVA:Query", B_BOOL_TYPE, 0, &Query, sizeof(Query)) == B_ENTRY_NOT_FOUND)
						throw new ProjectManagerException(new BString("an Attr. query doesn't exsist by some folder (LavaProjectManager::_readPorject)"), fObjDir, fNode);
					
					if(Query) {
						fEntry.SetTo(fPathAndFile->String());
						fEntry.GetParent(&fParentEntry);
						fParentEntry.GetPath(objTempPath);
						fNode->SetTo(objTempPath->Path());
						
						memset(buffer2, 0, sizeof(buffer2));
						if(fNode->ReadAttr("LAVA:PathToFolder", B_STRING_TYPE, 0, buffer2, sizeof(buffer2)) == B_ENTRY_NOT_FOUND)
							throw new ProjectManagerException(new BString("an Attr. PathToFolder doesn't exsist by some file (LavaProjectManager::_readPorject)"), fObjDir, fNode);

						int64 *tmp = new int64;
						*tmp = tmpSize;						
						fLavaProject->ProjectStructure->AddNode(buffer2, buffer, true, tmp, true);
					}
					else {
						
						int64 *tmp = new int64;
						*tmp = tmpSize;
						fLavaProject->ProjectStructure->AddNode(buffer2, buffer, false, tmp, false);
					}
					
					_readPorject(new BString(fPathAndFile->String()));
				}
	   		}
    	}
    }
  	
	if(dir != NULL)
		closedir(dir);
	
	delete dirzeiger, objTempPath;
}


/*	AddToPorject
*	Add files to an excisting project
*/
LavaProject*
LavaProjectManager::AddToPorject(FileTree *FileStructure, int DiscType)
{
	fProjectDiscType = DiscType;
	return AddToPorject(FileStructure);
}


/*	AddToPorject
*	Add files to an new created project
*/
LavaProject*
LavaProjectManager::AddToPorject(FileTree *FileStructure)
{
	fFileName = new BString();

	_writeToPorject(FileStructure);
	
	BString strTMP = "";
	strTMP = strPathToProject;
	strTMP += fActProject;
	
	fPathAndFile = new BString();
	fObjDir = new BDirectory();
	fObjFile = new BFile();
	fNode = new BNode();
		
	if(fProjectDiscType == -2) { //fProjectDiscType has his init value, that means that a project was created
		fLavaProject = new LavaProject(fActProject.String());
		fLavaProject->DiscType = fProjectDiscType;
	}
	else {
		int DiscType = -1;
		fNode->SetTo(strTMP.String());
		
		if(fNode->ReadAttr("LAVA:DiscType", B_INT32_TYPE, 0, &DiscType, sizeof(DiscType)) == B_ENTRY_NOT_FOUND)
			throw new ProjectManagerException(new BString("an Attr. PathToFolder doesn't exsist by some file (LavaProjectManager::_readPorject)"), fObjDir, fNode);
		
		if(fProjectDiscType != DiscType) {
		
			if(DiscType != fProjectDiscType && fLavaProject->ProjectSize > 734003200)
				fProjectDiscType = DATADVD;
			else if(DiscType != fProjectDiscType && fLavaProject->ProjectSize < 734003200)
				fProjectDiscType = DATACD;
		
			if(fNode->WriteAttr("LAVA:DiscType", B_INT32_TYPE, 0, &fProjectDiscType, sizeof(fProjectDiscType)) == B_FILE_ERROR)
				throw new ProjectManagerException(new BString("can't write attributes to project (LavaProjectManager::AddToPorject)"), fObjLavaFilePanel, fNode);
		}
		
		fLavaProject = new LavaProject(fActProject.String());
		fLavaProject->DiscType = fProjectDiscType;
	}
	
	_readPorject(new BString(strTMP.String()));
	
	delete fFileName, fPathAndFile, fObjDir, fObjFile, fNode;
	
	return fLavaProject;
}


/*	AddToPorject
*	write internal file structure (FileTree) to hard drive
*/
void
LavaProjectManager::_writeToPorject(FileTree *FileStructure)
{
	fObjDir = new BDirectory();
	fObjFile = new BFile();
	fNode = new BNode();
	BString Project = "";
	Project += strPathToProject;
	Project += fActProject;
	
	_writeToPorject(FileStructure->Nodes, new BString(Project.String()));
	
	delete fObjDir, fNode, fObjFile;
}


/*	AddToPorject
*	write internal file structure (FileTree) to hard drive
*/
void
LavaProjectManager::_writeToPorject(BList *nodes, BString *innerProject)
{
	
	FileTree *tmpTree;
	BString *ElementName = new BString("");
	BString *NextElement = new BString("");
	BString *RecentElement = new BString("");
	bool blIgnore = false;

	for(int i = 0; i < nodes->CountItems(); i++) {
		tmpTree = (FileTree*)nodes->ItemAt(i);
		
		if(tmpTree->File) { //File
			tmpTree->Path.CopyInto(*ElementName, tmpTree->Path.FindLast("/") + 1, tmpTree->Path.CountChars());
			tmpTree->Path.CopyInto(*RecentElement, tmpTree->Path.FindFirst("/"), tmpTree->Path.FindLast("/"));
			*RecentElement += "\0";
			
			NextElement->SetTo(*innerProject);
			*NextElement += "/";
			*NextElement += *ElementName;
			
			if(fObjFile->SetTo(NextElement->String(), B_READ_WRITE | B_CREATE_FILE | B_FAIL_IF_EXISTS) != B_OK) {
				fAlertTMP.SetTo("File already exists in project!\n\n");
				fAlertTMP += *NextElement;
				
				if((new BAlert("", fAlertTMP.String(), "ignore", "override",0, B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go() == 0)
					blIgnore = true;
				else {
					fEntry.SetTo(NextElement->String());
					fEntry.Remove();
					fObjFile->SetTo(NextElement->String(), B_READ_WRITE | B_CREATE_FILE | B_FAIL_IF_EXISTS);
				}
			}
			
			if(!blIgnore) {
				NextElement->CopyInto(*fFileName, NextElement->FindLast("/") + 1, NextElement->CountChars());
				fStateContainer->SetTo(fFileName->String());
				fNode->SetTo(NextElement->String());
				
				if(fNode->WriteAttr("LAVA:Parent", B_STRING_TYPE, 0, RecentElement->String(), RecentElement->Length()) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write parent attr. to project file (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);

				if(fNode->WriteAttr("LAVA:PathToFile", B_STRING_TYPE, 0, tmpTree->Path.String(), tmpTree->Path.Length()) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write PathToFile attr. to project file (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);
			
				if(fNode->WriteAttr("LAVA:Size", B_INT64_TYPE, 0, tmpTree->intSize, 8) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write Size attr. to project file (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);
				
				if(fNode->WriteAttr("LAVA:Query", B_BOOL_TYPE, 0, &tmpTree->Query, sizeof(tmpTree->Query)) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write query attr. to project file (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);
				
				if(fNode->WriteAttr("LAVA:Type", B_STRING_TYPE, 0, "file", sizeof("file")) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write Type attr. to project (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);
			}
			
			blIgnore = false;
		}
		else { //Folder
			fObjDir->SetTo(innerProject->String());
			tmpTree->Path.CopyInto(*ElementName, tmpTree->Path.FindLast("/") + 1, tmpTree->Path.CountChars());
			tmpTree->Path.CopyInto(*RecentElement, tmpTree->Path.FindFirst("/"), tmpTree->Path.FindLast("/"));
			*RecentElement += "\0";
			NextElement->SetTo(*innerProject);
			*NextElement += "/";
			*NextElement += *ElementName;	
			
			if(fObjDir->CreateDirectory(ElementName->String(), fObjDir) != B_OK) {
				fAlertTMP.SetTo("Folder ");
				fAlertTMP += *NextElement;
				fAlertTMP += " already exists in project and is ignored";
				(new BAlert("", fAlertTMP.String(), "OK"))->Go();
			}
			else {
				NextElement->CopyInto(*fFileName, NextElement->FindLast("/") + 1, NextElement->CountChars());
				fStateContainer->SetTo(fFileName->String());
				fNode->SetTo(NextElement->String());
				
				if(fNode->WriteAttr("LAVA:Parent", B_STRING_TYPE, 0, RecentElement->String(), RecentElement->Length()) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write parent attr. to project folder (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);
				
				if(fNode->WriteAttr("LAVA:PathToFolder", B_STRING_TYPE, 0, tmpTree->Path.String(), tmpTree->Path.Length()) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write PathToFolder attr. to project folder (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);
				
				if(fNode->WriteAttr("LAVA:Type", B_STRING_TYPE, 0, "subfolder", sizeof("subfolder")) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write Type attr. to project folder (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);

				if(fNode->WriteAttr("LAVA:Query", B_BOOL_TYPE, 0, &tmpTree->Query, sizeof(tmpTree->Query)) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write query attr. to project file (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);

				if(fNode->WriteAttr("LAVA:Size", B_INT64_TYPE, 0, tmpTree->intSize, 8) == B_FILE_ERROR)
					throw new ProjectManagerException(new BString("can't write Type attr. to project folder (LavaProjectManager::_writeToPorject())"), fObjDir, fNode, fObjFile, &fEntry);
				
				_writeToPorject(tmpTree->Nodes, new BString(NextElement->String()));
			}
		}
	}
	
	delete ElementName, NextElement, RecentElement, innerProject;
}
