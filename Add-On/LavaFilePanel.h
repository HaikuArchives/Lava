/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAFILEPANEL_H
#define LAVAFILEPANEL_H

#include <KernelKit.h>
#include <Messenger.h>
#include <Message.h>
#include <Handler.h>
#include <Looper.h>
#include <FilePanel.h>
#include <String.h>
#include <Path.h>
#include <Alert.h>

#define OPEN 'open'
#define SAVE 'save'

class LavaFilePanel: public BLooper {
	public:
		LavaFilePanel();
		~LavaFilePanel();
		
		virtual void MessageReceived(BMessage* message);
		void Save();
		void Open();
		
		BString *lfpPath;
		BString *lfpProject;
		
	private:
		sem_id _semLooper;
		bool _blSave_or_open;
};

#endif
