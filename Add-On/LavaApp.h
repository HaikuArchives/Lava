/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 *		Robert Stiehler, Negr0@team-maui.org
 */
 
#ifndef LAVAAPP_H
#define LAVAAPP_H

#define kAppSig		"application/x-vnd.Lava"
#define kMsgD 'datc'
#define kMsgDexternel 'ext'

/*#define CDIMAGE 0
#define DATACD 1
#define AUDIOCD 2
#define VCD 3
#define DATADVD 4
#define AUDIODVD 5
#define CUE 6
#define CDCOPY 7
#define DVDCOPY 8
#define EMPTYLIST 9
#define DVDIMAGE 10
#define ERROR -1
*/

#include <Application.h>
#include <MessageRunner.h>
#include <Window.h>

#include "LavaProjectWindow.h"
#include "LavaBurnStatusWindow.h"
#include "ProjectTypeSelector.h"

class App: public BApplication {
	public:
			App();
			~App();
			virtual		bool	QuitRequested();
			virtual		void	MessageReceived(BMessage* msg);	
	private:
			BString* fProjectNameString;
			BString* fWindowTitleString;			
};

#endif
