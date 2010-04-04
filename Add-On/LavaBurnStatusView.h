/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 */
 
#ifndef LAVABURNSTATUSVIEW_H
#define LAVABURNSTATUSVIEW_H
 
#include <InterfaceKit.h> 
#include <View.h>
#include <Message.h> 
#include <Bitmap.h>
#include <TranslationUtils.h>
#include <StatusBar.h>

#include "bitmaps/Bitmap_CD24.h"
//static const uint32 kMsgX = 'xxxx';

class BurnStatusView: public BView {
	public:
		BurnStatusView(BRect frame, int ViewType);
		~BurnStatusView();
		virtual void Draw(BRect updateRect);
		BStatusBar *fStatusBarBurn;
	private:
		BBitmap* fBitmapType;
		//BStatusBar* fStatusBarBurn;
		BButton* fButtonStop;
};

#endif

static const uint32 kStopButton = 'Tstp';

class TCustomButton : public BButton {
	public:
		TCustomButton(BRect frame, uint32 command);
		virtual	void Draw(BRect);
	private:
		typedef BButton _inherited;
};
