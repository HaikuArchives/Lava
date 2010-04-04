/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/
#include "LavaFilePanel.h"

LavaFilePanel::LavaFilePanel()
: BLooper("project"), _blSave_or_open(false), lfpPath(NULL), lfpProject(NULL)
{
	_semLooper = create_sem(0, "semLooper");
}


LavaFilePanel::~LavaFilePanel()
{
}

/*	overriting the MessageRecieved method for getting informations from
*	the BFilePanel which sends information (opened or saved file) as BMessage
*/
void
LavaFilePanel::MessageReceived(BMessage *message)
{
	entry_ref dir_ref;
	BEntry entry(&dir_ref);
	BPath path;
	
	switch(message->what) {
		case OPEN:
			_blSave_or_open = true;
			message->FindRef("refs", &dir_ref);
			entry.SetTo(&dir_ref);
			entry.GetPath(&path);
			lfpPath = new BString(path.Path());
			release_sem(_semLooper);
		break;
		case SAVE:
			_blSave_or_open = true;
			message->FindRef("directory", &dir_ref);
			entry.SetTo(&dir_ref);
			entry.GetPath(&path);
			lfpPath = new BString(path.Path());
			lfpProject = new BString();
			message->FindString("name", lfpProject);						
			release_sem(_semLooper);
		break;
		case B_CANCEL:
			if(!_blSave_or_open) {
				release_sem(_semLooper);
			}
		break;
		default:
		break;
	}
}


/*	Opens a BFilePanel in "modal" mode to save something
*/
void
LavaFilePanel::Save()
{	
	BFilePanel *objBFP = new BFilePanel(B_SAVE_PANEL, new BMessenger(this), 0, 0, false, new BMessage(SAVE), NULL, true);
	objBFP->SetSaveText("Save Project");
	objBFP->Show();
	acquire_sem(_semLooper);
	objBFP->Hide();
	delete objBFP;
}


/*	Opens a BFilePanel in "modal" mode to open something
*/
void
LavaFilePanel::Open()
{	
	BFilePanel *objBFP = new BFilePanel(B_OPEN_PANEL, new BMessenger(this), 0, B_DIRECTORY_NODE, false, new BMessage(OPEN), NULL, true);
	objBFP->Show();
	acquire_sem(_semLooper);
	objBFP->Hide();
	delete objBFP;
}
