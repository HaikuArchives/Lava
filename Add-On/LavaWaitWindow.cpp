/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 *		Robert Stiehler, Negr0@team-maui.org
 */
#include "LavaWaitWindow.h"

WaitWindow::WaitWindow(BMessage *msg, pid_t PID)
: BWindow(BRect(110, 80, 380, 180), "analyze files", B_TITLED_WINDOW, B_NOT_CLOSABLE | B_NOT_RESIZABLE | B_NOT_ZOOMABLE, 0)
{
	fParentID = PID;
	fQuited = false;
	fQuitStateThread = false;
	fSemQuit = create_sem(1, "fSemQuit");
	fSemQuitedAble = create_sem(1, "semHide");
	SetWorkspaces(B_ALL_WORKSPACES);
	fView = new WaitWindowView();
	fView->SetDrawingMode(B_OP_COPY);
	State = new BString();
	
	Lock();
		AddChild(fView);
		fView->fAnimationBox->Start(); //start the animation
	Unlock();
	
	Run();
	
	fStateThreadID = spawn_thread(_ReadState, "StateThread", B_LOW_PRIORITY, this);
	resume_thread(fStateThreadID);
}


WaitWindow::~WaitWindow()
{
}


bool
WaitWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);	
	return true;
}


int32
WaitWindow::_DelayThread(void *data)
{	
	WaitWindow *WaitWin = (WaitWindow*)data;
	sleep(1);
	
	acquire_sem(WaitWin->fSemQuit);
	if(!WaitWin->fQuited)
		WaitWin->Show();
	release_sem(WaitWin->fSemQuit);
	
	release_sem(WaitWin->fSemQuitedAble);
		
	return 0;
}


BString*
WaitWindow::getStateContainer()
{
	return State;
}


int32
WaitWindow::_ReadState(void *data)
{
	WaitWindow *WaitWin = (WaitWindow*)data;
	
	while(!WaitWin->fQuitStateThread) {
		snooze(15000);
		WaitWin->fView->LockLooper();
		WaitWin->fView->StateMessageView->SetText(WaitWin->State->String());
		WaitWin->fView->UnlockLooper();
	}
	
	return 0;
}


void
WaitWindow::StartWait()
{
	acquire_sem(fSemQuitedAble);
	fWaitWinThreadID = spawn_thread(_DelayThread, "DelayThread", B_NORMAL_PRIORITY, this);
	resume_thread(fWaitWinThreadID);
}


void
WaitWindow::HideWait()
{
	Hide();
}


void
WaitWindow::QuitWait()
{
	Hide();
	
	fQuitStateThread = true;
	status_t exit_value;
	wait_for_thread(fStateThreadID, &exit_value);
	
	acquire_sem(fSemQuitedAble);
	
	acquire_sem(fSemQuit);
	fQuited = true;
	release_sem(fSemQuit);
	Quit();
}


void
WaitWindow::MessageReceived(BMessage* msg)
{	
	switch (msg->what) {	
		case 'canc': {	
			kill_thread(fStateThreadID);
			kill_thread(fParentID);
			Quit();
			break;
		}
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


WaitWindowView::WaitWindowView()
: BView(BRect(0, 0, 270, 100), "", B_FOLLOW_ALL, B_WILL_DRAW)
{	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// AnimationBox
	BRect AnimationBoxRect = BRect(BPoint(Bounds().left + 15, Bounds().top + 15), BPoint(Bounds().right - 16, Bounds().top + 30));
	fAnimationBox = new AnimationBox(AnimationBoxRect, "", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS);
	AddChild(fAnimationBox);
	
	//StateMessageView
	BRect StateMessageViewRect = BRect(BPoint(Bounds().left + 15, Bounds().top + 35), BPoint(Bounds().right - 16, Bounds().top + 55));
	StateMessageView = new BStringView(StateMessageViewRect, "Message", "", B_FOLLOW_ALL, B_WILL_DRAW);
	AddChild(StateMessageView);
	
	//CancelButton
	BRect CancelButtonRect = BRect(BPoint(Bounds().left + 190, Bounds().top + 70), BPoint(Bounds().right - 16, Bounds().top + 90));
	fCancelButton = new BButton(CancelButtonRect, "CancelButton", "Cancel", new BMessage('canc'), B_FOLLOW_ALL, B_WILL_DRAW);
	AddChild(fCancelButton);
}


WaitWindowView::~WaitWindowView()
{
}


void
WaitWindowView::Draw(BRect updatetRect)
{

	const rgb_color  darker_grey  =  {108, 108, 108, 255};
	const rgb_color  plain_white  =  {255, 255, 255, 255};
	const rgb_color  deep_black  =  {255, 255, 255, 255};

	// divider 
	SetHighColor(darker_grey);
	StrokeLine(BPoint(Bounds().left + 15, Bounds().top + 61), BPoint(Bounds().right - 16, Bounds().top + 61));
	
	SetHighColor(plain_white);
	StrokeLine(BPoint(Bounds().left + 15, Bounds().top + 62), BPoint(Bounds().right - 16, Bounds().top + 62));

}
