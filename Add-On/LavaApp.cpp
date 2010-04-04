/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 *		Robert Stiehler, Negr0@team-maui.org
 */

#include <Alert.h> 
#include <Roster.h>
#include <List.h>
#include <Rect.h>
#include <string.h>
#include <String.h>
#include <Path.h>
#include "LavaApp.h"


extern "C" void 
process_refs(entry_ref dir_ref, BMessage *msg, void *);


App::App()
: BApplication(kAppSig)
{
}

App::~App()
{
}

bool
App::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);	
	return true;
}


void
App::MessageReceived(BMessage* msg)
{
	switch(msg->what) {
		case kMsgD: {
			float kX = 100;
			float kY = 100;
			BRect BrunStatusRect;
			BrunStatusRect.Set(kX, kY, kX + 350, kY + 50);
			BRect ProjectWindowRect;
			ProjectWindowRect.Set(kX, kY, kX + 250 + 100, kY + 350); // kX+400,kY+210
			
			int32 DiscType;
			msg->FindInt32("type", &DiscType);
			/*
			// Debug
			BString *strDiscType = new BString();
			*strDiscType << "Selectet DiscType: " << DiscType;
			(new BAlert("", strDiscType->String(), "Exit"))->Go();
			//
			*/
			
			switch(DiscType){
				case CDIMAGE: {
					//(new BAlert("", "CD Image", "Exit"))->Go();
					BurnStatusWindow* win = new BurnStatusWindow(BrunStatusRect, 0, msg);
					win->SetTitle("Burn Status");
					//win->SetLook(B_FLOATING_WINDOW_LOOK);
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case DVDIMAGE: {
					//(new BAlert("", "DVD Image", "Exit"))->Go();
					BurnStatusWindow* win = new BurnStatusWindow(BrunStatusRect, 0, msg);
					win->SetTitle("Burn Status");
					//win->SetLook(B_FLOATING_WINDOW_LOOK);
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case PORJECT:
				case DATACD: {
					const char strProjectType[100] = "Type: Data CD";
					msg->AddString("projecttype", (BString)strProjectType);
					
					ProjectWindow* win = new ProjectWindow(ProjectWindowRect, 0, msg);
					
					fProjectNameString = new BString();
					fWindowTitleString = new BString();
					msg->FindString("PojectName", fProjectNameString);
					*fWindowTitleString <<"Project: " <<fProjectNameString->String();
					win->SetTitle(fWindowTitleString->String());
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case AUDIOCD: {
					//(new BAlert("", "Audio CD", "Exit"))->Go();
					ProjectWindow* win = new ProjectWindow(ProjectWindowRect, 0, msg);
					
					fProjectNameString = new BString();
					fWindowTitleString = new BString();
					msg->FindString("PojectName", fProjectNameString);
					*fWindowTitleString <<"Project: " <<fProjectNameString->String();
					win->SetTitle(fWindowTitleString->String());
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case VCD: {
					//(new BAlert("", "VCD", "Exit"))->Go();
					BurnStatusWindow* win = new BurnStatusWindow(BrunStatusRect, 0, msg);
					win->SetTitle("Burn Status");
					//win->SetLook(B_FLOATING_WINDOW_LOOK);
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case DATADVD: {
					//(new BAlert("", "Daten DVD", "Exit"))->Go();
					ProjectWindow* win = new ProjectWindow(ProjectWindowRect, 0, msg);
					
                    fProjectNameString = new BString();
					fWindowTitleString = new BString();
					msg->FindString("PojectName", fProjectNameString);
					*fWindowTitleString <<"Project: " <<fProjectNameString->String();
					win->SetTitle(fWindowTitleString->String());
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case AUDIODVD: {
					//(new BAlert("", "Audio DVD", "Exit"))->Go();
					ProjectWindow* win = new ProjectWindow(ProjectWindowRect, 0, msg);
					
                    fProjectNameString = new BString();
					fWindowTitleString = new BString();
					msg->FindString("PojectName", fProjectNameString);
					*fWindowTitleString <<"Project: " <<fProjectNameString->String();
					win->SetTitle(fWindowTitleString->String());
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case CUE: {
					//(new BAlert("", "CUE", "Exit"))->Go();
					BurnStatusWindow* win = new BurnStatusWindow(BrunStatusRect, 0, msg);
					win->SetTitle("Burn Status");
					//win->SetLook(B_FLOATING_WINDOW_LOOK);
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case CDCOPY: {
					//(new BAlert("", "CD kopieren", "Exit"))->Go();
					BurnStatusWindow* win = new BurnStatusWindow(BrunStatusRect, 0, msg);
					win->SetTitle("Burn Status");
					//win->SetLook(B_FLOATING_WINDOW_LOOK);
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				case DVDCOPY: {
					//(new BAlert("", "DVD kopieren", "Exit"))->Go();
					BurnStatusWindow* win = new BurnStatusWindow(BrunStatusRect, 0, msg);
					win->SetTitle("Burn Status");
					//win->SetLook(B_FLOATING_WINDOW_LOOK);
					
					//Lock();
					win->Show();
					//Unlock();
					break;
				}
				default: {
					BApplication::MessageReceived(msg);
					(new BAlert("", "Error", "Exit"))->Go();
					break;
				}
			}
			break;
		}
		case kMsgDexternel: {
			entry_ref dir_ref;
			process_refs(dir_ref, msg, NULL);
			break;
		}
		default: {
			BApplication::MessageReceived(msg);
			break;
		}
	};
};
