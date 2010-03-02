/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 */
 
#ifndef LAVAPROJECTWINDOW_H
#define LAVAPROJECTWINDOW_H

#include <Window.h>
#include <View.h>
#include <StringView.h>
#include <Message.h> 
#include <Menu.h> 
#include <MenuField.h> 

#include "Logging.h"
#include "ColumnTypes.h"
#include "ColumnListView.h"
#include "ProjectTypeSelector.h"

class ProjectWindow: public BWindow
{
	public:
			ProjectWindow(BRect frame, int WindowType, BMessage *msg);
			~ProjectWindow();
			
			virtual     bool    QuitRequested();
			virtual     void	MessageReceived(BMessage* msg);	
			BView* fMainView;
			BColumnListView* fColumnView;
			BView* fStatusView;
			BStringView* fStatusStringView;
			BStringColumn* fStringColumnName;
			
			BBitmapColumn* fIconColumnName;
			
			BSizeColumn* fSizeColumnSize;
			BButton* fBurnButton;
			BMenu* fMenuType;		
			BMenuField*	fMenuFieldTypeItem;	
			
			
			
	private:
			//	
			entry_ref fFile_ref;
			BEntry fEntry;
			BPath fPath;
			char fName[B_FILE_NAME_LENGTH];
			
			off_t fItemSize;
			BRow* fRow;
			BStringField* fStringField;
			
			BBitmapField* fIconField;
			
			BSizeField* fSizeField;
			BString *fFileSizeString;
			off_t fFullItemsSize;	
			
			BMenuItem* fMenuItemDataCD; 
			BMenuItem* fMenuItemAudioCD; 
			BMenuItem* fMenuItemDataDVD;
			BMenuItem* fMenuItemAudioDVD;
			
			Logging *fobjLogging;
};

#endif
