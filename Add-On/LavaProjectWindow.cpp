/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 *		Robert Stiehler, Negr0@team-maui.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <unistd.h>
#include <signal.h>

#include <Application.h>
#include <InterfaceKit.h> 
#include <StorageKit.h> 

#include "LavaProjectWindow.h"

ProjectWindow::ProjectWindow(BRect frame,int WindowType, BMessage *msg)
: BWindow(frame, "Lava", B_TITLED_WINDOW,B_NOT_RESIZABLE | B_NOT_ZOOMABLE, 0)
{
	fobjLogging = new Logging();
	fobjLogging->WriteAsAttribute();
	fobjLogging->setLogfileDirectory(B_USER_SETTINGS_DIRECTORY);
	fobjLogging->setLogfileName("Lava");

	float kX = frame.Width() - frame.Width();
	float kY = frame.Height() - frame.Height();
	float kW = frame.Width();
	float kH = frame.Height();	
	frame.Set(kX, kY, kW, kH);
	
	const rgb_color kRGB1 = {255, 255, 255, 255};
	const rgb_color kRGB2 = {195, 195, 195, 255};
	
	FileAccess objFileAccess;
	
	switch (WindowType) {
		case 0: {
			// ProjectWindow
			
			_unarchivProjectFromAddon(msg);			
			
			//MainView
			fMainView = new BView(frame, "", B_FOLLOW_ALL, B_WILL_DRAW);
			fMainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
			
			//ColumnView 
			fColumnView = new BColumnListView(BRect(kX + 10, kY + 10, kW - 10, kH - 53), "", B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE, B_PLAIN_BORDER, true);
			fColumnView->SetColor(B_COLOR_BACKGROUND, kRGB1);
			fColumnView->SetColor(B_COLOR_ROW_DIVIDER, kRGB1);
			fColumnView->SetColor(B_COLOR_SELECTION, kRGB2);
			fColumnView->SetSelectionMode(B_SINGLE_SELECTION_LIST);
			
			fStatusView = new BView(Bounds(), "", B_FOLLOW_ALL, B_WILL_DRAW);
			fStatusView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
			fStatusStringView = new BStringView(BRect(0, 0, 152, 15), "size", "Size:");
			fStatusView->AddChild(fStatusStringView);
			fColumnView->AddStatusView(fStatusView);
			
			fIconColumnName = new BBitmapColumn("", (float)20, (float)20, (float)20, B_ALIGN_CENTER);
			fStringColumnName = new BStringColumn("name", 200 - 20, 30, 400, 0, B_ALIGN_LEFT);
			fSizeColumnSize = new BSizeColumn("size", 92 - 25, 30, 92, B_ALIGN_LEFT);
			
			fColumnView->AddColumn(fIconColumnName, 0);
			fColumnView->AddColumn(fStringColumnName, 1);
			fColumnView->AddColumn(fSizeColumnSize, 2);
			AddChild(fColumnView);
			
			//MenuBox
			fMenuType = new BMenu("Select"); //Dummy
			fMenuType->SetLabelFromMarked(true);
			fMenuFieldTypeItem = new BMenuField(BRect(10, kY + kH - 35, 250, kH - 15), "projecttype", "Type: ", fMenuType);
			fMenuFieldTypeItem->SetDivider(be_plain_font->StringWidth("Type: "));
			fMenuItemDataCD = new BMenuItem("Data CD", new BMessage('dcd'), 0, 0);
			fMenuItemAudioCD = new BMenuItem("Audio CD", new BMessage('acd'), 0, 0);
			fMenuItemDataDVD= new BMenuItem("Data DVD", new BMessage('ddvd'), 0, 0);
			fMenuItemAudioDVD = new BMenuItem("Audio DVD", new BMessage('advd'), 0, 0);
			fMainView->AddChild(fMenuFieldTypeItem);
			
			int32 DiscType;
			msg->FindInt32("type", &DiscType);
			
			switch(DiscType) {
				case DATACD: {
					fMenuType->AddItem(fMenuItemDataCD);
					//fMenuType->AddItem(fMenuItemAudioCD);
					fMenuItemDataCD->SetMarked(true);
					break;
				}
				case AUDIOCD: {
					fMenuType->AddItem(fMenuItemDataCD);
					fMenuType->AddItem(fMenuItemAudioCD);
					fMenuItemAudioCD->SetMarked(true);
					break;
				}
				case DATADVD: {
					fMenuType->AddItem(fMenuItemDataDVD);
					//fMenuType->AddItem(fMenuItemAudioDVD);
					fMenuItemDataDVD->SetMarked(true);
					break;
				}
				case AUDIODVD: {
					fMenuType->AddItem(fMenuItemDataDVD);
					fMenuType->AddItem(fMenuItemAudioDVD);
					fMenuItemAudioDVD->SetMarked(true);
					break;
				}
				default:
					/*fMenuType->AddItem(fMenuItemDataCD);
					fMenuType->AddItem(fMenuItemAudioCD);
					fMenuType->AddItem(fMenuItemDataDVD);
					fMenuType->AddItem(fMenuItemAudioDVD);*/
					break;
			}
						
			//BurnButton
			fBurnButton = new BButton(BRect(kX + kW - 100, kY + kH - 35, kW - 10, kH - 15), "BurnProject", "Burn", new BMessage('burn') , B_FOLLOW_ALL, B_WILL_DRAW);
			fMainView->AddChild(fBurnButton);
			
			//Selectec Files, initalisiert in Lava.cpp	
			for(int refs = 0; msg->FindRef("refs", refs, &fFile_ref) == B_NO_ERROR; refs++) {
				fEntry.SetTo(&fFile_ref);
				fEntry.GetPath(&fPath);
				fEntry.GetName(fName);
				
				msg->FindInt64(fPath.Path(), &fItemSize);
				
				fRow = new BRow(20.0);
				
				BBitmap* BIcon;
				BIcon = objFileAccess.getIconOfFile(fPath.Path());
				fIconField = new BBitmapField(BIcon);
				fRow->SetField(fIconField, 0);
				
				fStringField = new BStringField(fName);
				fRow->SetField(fStringField, 1);
				
				fSizeField = new BSizeField(fItemSize);
				fRow->SetField(fSizeField, 2);
					
				fColumnView->AddRow(fRow, (int32)refs);
				//fColumnView->SetSortColumn(fStringColumnName,true, true); //sort by name
				fColumnView->SetSortColumn(fSizeColumnSize, true, false); //sort by size
			}
			
			fFileSizeString = new BString();
			msg->FindInt64("size", &fFullItemsSize);
			
			if(fFullItemsSize > 1048576) {
				fFullItemsSize = ((fFullItemsSize / 1024) / 1024); //Umrechnung in MB
				*fFileSizeString <<fColumnView->CountRows(0) <<" Items / " <<(int64)fFullItemsSize <<" " <<"MB";
			}
			else {
				if(fFullItemsSize > 1024) {
					fFullItemsSize = fFullItemsSize/1024;
					*fFileSizeString <<fColumnView->CountRows(0) <<" Items / "<<(int64)fFullItemsSize <<" " <<"KB";
				}
				else {
					*fFileSizeString <<fColumnView->CountRows(0) <<" Items / "<<(int64)fFullItemsSize <<" " <<"bytes";
				}
			}
			
			fStatusStringView->SetText(fFileSizeString->String());
	
			Lock();
			AddChild(fMainView);
			Unlock();
			break;
		}
		case 1: {	
			// 
		}
	}
}

ProjectWindow::~ProjectWindow()
{
}

bool
ProjectWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);	
	return true;
}

void
ProjectWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case 'burn': {
		// test stuff for developing of burning device (so it is much easy to debug it)
		(new BAlert("", "Start Burning", "Exit"))->Go(); //Test Alert
		//BurnDevice *tst = new BurnDevice(true, this, this);
		//tst->SetDao(true);
		
			break;
		}
		case 'dcd': {
			/*TODO: When the Items have mp3,OGG ore other Audio formats, then
			can switch to AudioCD and the not Audiofiles are deleate from the list */
			(new BAlert("", "Data CD", "Exit"))->Go(); //Debug Alert
			break;
		}
		case 'acd': {
			/*TODO: When switch to DataCD, then convert the wavfiles to mp3 */
			(new BAlert("", "Audio CD", "Exit"))->Go(); //Debug Alert
			break;
		}
		case 'ddvd': {
			 /*TODO: same as DATACD */
			(new BAlert("", "Data DVD", "Exit"))->Go(); //Debug Alert
			break;
		}
		case 'advd': {
			 /*TODO: same as AUDIOCD */
			(new BAlert("", "Audio DVD", "Exit"))->Go(); //Debug Alert
			break;
		}
		case 'abca': {
			(new BAlert("", "Message from burn device", "Exit"))->Go(); //Test Alert
			break;
		}
		case 'log': {
			BString tmpString;// = new BString();
			msg->FindString("DebugMsg", &tmpString);
			fobjLogging->LogString(tmpString.String());
			break;
		}
		default:
			BWindow::MessageReceived(msg);
			break;
	};
}

void
ProjectWindow::_unarchivProjectFromAddon(BMessage *msg) {
	fLavaProject = reinterpret_cast<LavaProject*>(LavaProject::Instantiate(msg));
}
