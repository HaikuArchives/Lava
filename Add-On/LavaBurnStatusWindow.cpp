/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 */
#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <unistd.h>
#include <signal.h>

#include <Application.h>
#include <InterfaceKit.h> 
#include <StorageKit.h> 

#include "FileAccess.h"
#include "LavaBurnStatusWindow.h"
//#include "LavaBurnStatuView.h"
//frame = (100,100,500,270);

/*TODO: Here need more to work, set Label from the FileItems read the BurnStatusValue from BurnDevice in the BurnProgress etc. */

BurnStatusWindow::BurnStatusWindow(BRect frame, int WindowType, BMessage *msg)
: BWindow(frame, "Lava", B_TITLED_WINDOW, B_NOT_CLOSABLE | B_NOT_RESIZABLE | B_NOT_ZOOMABLE, 0)
{
	float kX = frame.Width() - frame.Width();
	float kY = frame.Height() - frame.Height();
	float kW = frame.Width();
	float kH = frame.Height();	
	frame.Set(kX, kY, kW, kH);
	
	FileAccess objFileAccess;
	
	switch (WindowType) {
		case 0: {
			// StatusBurnWindow
			fView = new BurnStatusView(frame, WindowType);
			//fView->SetDrawingMode(B_OP_ALPHA);
			
			//Selectec Files, initalisiert in Lava.cpp	
			for(int refs = 0; msg->FindRef("refs", refs, &fFile_ref) == B_NO_ERROR; refs++) {
				fEntry.SetTo(&fFile_ref); 
				fEntry.GetPath(&fPath);
				fEntry.GetName(fName);
				
				//SetLabel
				fView->fStatusBarBurn->SetText(fName);
				fView->fStatusBarBurn->SetTrailingText("0%/100%");
			}
			
			Lock();
			AddChild(fView);
			Unlock();
			break;
		}
		case 1: {
		}	
	}	
}

BurnStatusWindow::~BurnStatusWindow()
{
}

bool
BurnStatusWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);	
	return true;
}

void
BurnStatusWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {	
		case kStopButton: {
			//BAlert* TestAlert = new BAlert("","xxx","EXIT");
			//TestAlert->Go();
			QuitRequested();
			break;		
		}
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	};
};

