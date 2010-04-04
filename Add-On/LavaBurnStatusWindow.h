/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 */
 
#ifndef LAVABURNSTATUSWINDOW_H
#define LAVABURNSTATUSWINDOW_H

#include <Window.h>

#include "LavaBurnStatusView.h"

class BurnStatusWindow: public BWindow {
	public:
		BurnStatusWindow(BRect frame, int WindowType, BMessage *msg);
		~BurnStatusWindow();
		virtual bool QuitRequested();
		virtual void MessageReceived(BMessage* msg);		
	private:
		BurnStatusView *fView;
		
		entry_ref fFile_ref;
		BEntry fEntry;
		BPath fPath;
		char fName[B_FILE_NAME_LENGTH];		
};

#endif
