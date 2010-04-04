/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 *		Robert Stiehler, Negr0@team-maui.org
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <TrackerAddOn.h>
#include <Alert.h>
#include <Roster.h>
#include <List.h>

#include "Logging.h"
#include "FileTree.h"
#include "FileAccess.h"
#include "ProjectTypeSelector.h"
#include "LavaApp.h"
#include "LavaProjectManager.h"
#include "settings/LavaSettings.h"
#include "LavaWaitWindow.h"
#include "Exceptions/Exception.h"
#include "Exceptions/LavaMainException.h"
#include "Exceptions/IOSettingsException.h"
#include "Exceptions/LoggingException.h"

LavaProject* create_or_open_Project(WaitWindow *fWaitWindow, FileTree *objFileTree, BMessage *objMsg, off_t *Size, BString actPro, int *DiscType)
{
	LavaProject *objProject;
	LavaProjectManager *objLavaProjectManager = new LavaProjectManager();
	objLavaProjectManager->setStateContainer(fWaitWindow->getStateContainer());
	
	if(actPro.Compare("") == 0) {
		fWaitWindow->HideWait();
		
		switch((new BAlert("", "Creat a new project or open a existing?", "Cancel", "New", "Open"))->Go()) {
			case 1: { //New project
				objLavaProjectManager->CreateNewProject(*DiscType);
				fWaitWindow->StartWait();
				objProject = objLavaProjectManager->AddToPorject(objFileTree);
				break;
			}
			case 2: { //Open project
				objLavaProjectManager->OpenPorject();
				fWaitWindow->StartWait();
				objProject = objLavaProjectManager->AddToPorject(objFileTree, *DiscType);
				
				if(*DiscType != objProject->DiscType && *Size > 734003200)
					*DiscType = DATADVD;
				else if(*DiscType != objProject->DiscType && *Size < 734003200)
					*DiscType = DATACD;
				
				break;
			}
			default: { //Cancel
				throw new LavaMainException(new BString("adding files to project canceled"));
				break;
			}
		}
		
		objProject->ProjectStructure->AddTreeToBMsg(objMsg);
		objProject->ProjectStructure->AddFisrtInstanceAsRefToBMsg(objMsg);
		*Size = objProject->ProjectSize;
		objMsg->AddString("PojectName", (objProject->getProjectName())->String());
		objMsg->AddString("ProjectPath", objLavaProjectManager->getActProjectPath());
	}
	else {
		objProject = objLavaProjectManager->OpenPorject(actPro);
		
		if(*DiscType != PORJECT)
			objProject = objLavaProjectManager->AddToPorject(objFileTree, *DiscType);
		
		objProject->ProjectStructure->AddTreeToBMsg(objMsg);
		objProject->ProjectStructure->AddFisrtInstanceAsRefToBMsg(objMsg);
		objMsg->AddString("PojectName", (objProject->getProjectName())->String());
		objMsg->AddString("ProjectPath", objLavaProjectManager->getActProjectPath());
		*Size = objProject->ProjectSize;
				
		if(*DiscType != PORJECT && (*DiscType != objProject->DiscType && *Size > 734003200))
			*DiscType = DATADVD;
		else if(*DiscType != PORJECT && (*DiscType != objProject->DiscType && *Size < 734003200))
			*DiscType = DATACD;
		else
			*DiscType = objProject->DiscType;
	}
	
	//copy lava project to BMessage
	objProject->Archive(objMsg);
	
	return objProject;
}

extern "C" void
process_refs(entry_ref dir_ref, BMessage *msg, void *) 
{
	//Wait Window
	WaitWindow *fWaitWindow;
	Logging *objLogging = new Logging();
	
	try {
		objLogging->WriteAsAttribute();
		objLogging->setLogfileDirectory(B_USER_SETTINGS_DIRECTORY);
		objLogging->setLogfileName("Lava");
		fWaitWindow = new WaitWindow(msg, find_thread(NULL));
		fWaitWindow->StartWait();
		
		BPath path; 
		BEntry entry;
		int refs; 
		entry_ref file_ref;
		off_t Size = 0;
		int DiscType = 0;
		BMessage *objMsg = new BMessage(kMsgD);
		
		BList FileList;
		ProjectTypeSelector *objProjectTypeSelector = new ProjectTypeSelector();
		FileAccess *objFileAccess = new FileAccess();
		objFileAccess->setStateContainer(fWaitWindow->getStateContainer());
		FileTree *objFileTree;
		
		for(refs = 0; msg->FindRef("refs", refs, &file_ref) == B_NO_ERROR; refs++) {
			entry.SetTo(&file_ref); 
			entry.GetPath(&path);
			FileList.AddItem(new BString(path.Path()));
		}
		
		objFileAccess->setFiles(&FileList);
		objProjectTypeSelector->setFileAccess(objFileAccess);
		DiscType = objProjectTypeSelector->getDiscType();
		
		if(DiscType != EMPTYLIST) {
			objFileAccess->generateFileInfos();
			objFileTree = objFileAccess->getFileTree();	
			
			if(DiscType == CDIMAGE || DiscType == VCD || DiscType == CUE || DiscType == CDCOPY || DiscType == DVDCOPY || DiscType == DVDIMAGE) {
				objFileTree->AddTreeToBMsg(objMsg);
				objFileTree->AddFisrtInstanceAsRefToBMsg(objMsg);
				Size = objFileAccess->getFileSize();
			}
			else if(DiscType == PORJECT) {
				LavaSettings *objIOSettings = new LavaSettings();
				BString *Project = (BString*)FileList.FirstItem();
				BString *actProject = (BString*)objIOSettings->ReadSetting("LAVA:ActProject");
				
				if(Project->Compare(*actProject) == 0) {
					objIOSettings->WriteSetting("LAVA:ActProject", "");
					fWaitWindow->HideWait();
					throw new LavaMainException(new BString("project is now deactive"));
				}
				else {
					objIOSettings->WriteSetting("LAVA:ActProject", Project->String());
					fWaitWindow->HideWait();
					(new BAlert("", "project is now active", "Exit"))->Go();
					create_or_open_Project(fWaitWindow, objFileTree, objMsg, &Size, Project->String(), &DiscType);
				}				
			}
			else {
				LavaSettings *objIOSettings = new LavaSettings();
				BString *actProject = (BString*)objIOSettings->ReadSetting("LAVA:ActProject");
				
				if(actProject->Compare("") == 0) {
					create_or_open_Project(fWaitWindow, objFileTree, objMsg, &Size, "", &DiscType);
				}
				else {
					try {
						create_or_open_Project(fWaitWindow, objFileTree, objMsg, &Size, actProject->String(), &DiscType);
					}
					catch(...) {
						objIOSettings->WriteSetting("LAVA:ActProject", "");
						create_or_open_Project(fWaitWindow, objFileTree, objMsg, &Size, "", &DiscType);
					}
				}
			
			}
			
			objMsg->AddInt64("size", Size);
			objMsg->AddInt32("type", DiscType);
			
			fWaitWindow->QuitWait();
			
			app_info AppInfo;
			if(be_roster->GetAppInfo(kAppSig, &AppInfo) != B_OK) {
				status_t ret = be_roster->Launch(kAppSig, objMsg);
				if (ret < B_OK) 
					throw new LavaMainException(new BString("error during launching"));
			
			}
			else {
				BMessenger objMessanger(kAppSig);
				if(objMessanger.SendMessage(objMsg) != B_OK)
					throw new LavaMainException(new BString("error during launching"));
			}
		}
		else {
			fWaitWindow->HideWait();
			throw new LavaMainException(new BString("no files seleceted"));
		}
	}
	catch(ProjectTypeSelectorException *e) {
		(new BAlert("", e->ExceptionText->String(), "Exit"))->Go();
		objLogging->LogString(e->ExceptionText->String());
		delete e;
		fWaitWindow->QuitWait();
	}
	catch(IOSettingsException *e) {
		(new BAlert("", e->ExceptionText->String(), "Exit"))->Go();
		objLogging->LogString(e->ExceptionText->String());
		delete e;
		fWaitWindow->QuitWait();
	}
	catch(ProjectManagerException *e) {
		(new BAlert("", e->ExceptionText->String(), "Exit"))->Go();
		objLogging->LogString(e->ExceptionText->String());
		delete e;
		fWaitWindow->QuitWait();
	}
	catch(LavaMainException *e) {
		(new BAlert("", e->ExceptionText->String(), "Exit"))->Go();
		objLogging->LogString(e->ExceptionText->String());
		delete e;
		fWaitWindow->QuitWait();
	}
	catch(LoggingException *e) { //exception which can not be logged ;)
		(new BAlert("", e->ExceptionText->String(), "Exit"))->Go();
		delete e;
		fWaitWindow->QuitWait();
	}
	catch(...) {
		(new BAlert("", "unhandled exception!", "Exit"))->Go();
		objLogging->LogString("unhandled exception!");
		fWaitWindow->QuitWait();
	}
}

int
main(int argc, char * argv[])
{
	App::App app;
	app.Run();
	
	return 0;
}
