/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 *		Robert Stiehler, Negr0@team-maui.org
 */
 
#ifndef LAVAWAITWINDOW_H
#define LAVAWAITWINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <unistd.h>
#include <signal.h>
#include <InterfaceKit.h> 
#include <StorageKit.h> 
#include <Window.h>
#include <View.h>
#include <OS.h> 

#include "Exceptions/Exception.h"
#include "Exceptions/LavaMainException.h"
#include "LavaWaitAnimationBox.h"

class WaitWindowView;

class WaitWindow: public BWindow {
	public:
		WaitWindow(BMessage *msg, pid_t PID);
		~WaitWindow();
		virtual bool QuitRequested();
		virtual void MessageReceived(BMessage* msg);
		void HideWait();
		void StartWait();
		void QuitWait();
		BString* getStateContainer();
		BString *State;
	private:
		pid_t fParentID;
		static int32 _DelayThread(void *data);
		static int32 _ReadState(void *data);
		bool fQuited;
		sem_id fSemQuit;
		sem_id fSemQuitedAble;
		WaitWindowView* fView;
		thread_id fWaitWinThreadID;
		bool fQuitStateThread;
		thread_id fStateThreadID;
};


class WaitWindowView: public BView {
	public:
		WaitWindowView();
		~WaitWindowView();
		AnimationBox *fAnimationBox;
		BStringView *StateMessageView;
		virtual void Draw(BRect updatetRect);
	private:
		BButton* fCancelButton;
			
};

#endif
