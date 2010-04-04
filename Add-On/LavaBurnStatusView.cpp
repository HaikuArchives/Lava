/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ralf Schülke, teammaui@web.de
 */
 
//#include <InterfaceKit.h> 
#include <stdio.h> 
#include <string.h> 
#include <String.h> 

#include "LavaBurnStatusView.h"

BurnStatusView::BurnStatusView(BRect frame, int ViewType)
: BView(frame, "", B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	float kX = frame.Width() - frame.Width();
	float kY = frame.Height() - frame.Height();
	float kW = frame.Width();
	float kH = frame.Height();	
	//kX+10,kY+10,kW-25,kH-85	
	
	switch (ViewType) {
		case 0: {
			fBitmapType = new BBitmap(BRect(0, 0, 23, 23), B_RGBA32);
			/* SetBits() methode runs not corectly in zeta and BeOSr5, but under Haiku :-), 
			also we used in time the memcpy() methode. THX AxelD for helping and give me the memcpy() */
			//fBitmapType->SetBits((const void *)kBitmap_CD24Bits, (int32)fBitmapType->BitsLength(), 0, B_RGBA32);
			memcpy(fBitmapType->Bits(), kBitmap_CD24Bits, fBitmapType->BitsLength());
			
			//BStatusBar
			fStatusBarBurn = new BStatusBar(BRect(39, 7, kW - 35, 40), "status", "", "");
			fStatusBarBurn->SetBarHeight(12);
			AddChild(fStatusBarBurn);
			//BButton for stop the Burning
			//fButtonStop = new BButton(BRect(kW-30,19.5,kW-5,0),"burn_stop","X",new BMessage('stop') ,B_FOLLOW_ALL,B_WILL_DRAW);
			BRect rect(kW - 30, 19.5, kW - 5, 0);
			fButtonStop = new TCustomButton(rect, kStopButton);
			fButtonStop->ResizeTo(22, 18);
			AddChild(fButtonStop);
			break;
		}
		case 1: {
			break;
		}	
	}
}


BurnStatusView::~BurnStatusView()
{
}


void
BurnStatusView::Draw(BRect updateRect)
{
	SetDrawingMode(B_OP_ALPHA);
	DrawBitmap(fBitmapType,BPoint(5,10));	
}

//Custom the Stop Button
TCustomButton::TCustomButton(BRect frame, uint32 what)
: BButton(frame, "", "", new BMessage(what), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW)
{
}


void
TCustomButton::Draw(BRect updateRect)
{
	_inherited::Draw(updateRect);

	if (Message()->what == kStopButton) {
		updateRect = Bounds();
		updateRect.InsetBy(9, 7);
		SetHighColor(0, 0, 0);
		FillRect(updateRect);
	}
	else {
		updateRect = Bounds();
		updateRect.InsetBy(9, 6);
		BRect rect(updateRect);
		rect.right -= 3;

		updateRect.left += 3;
		updateRect.OffsetBy(1, 0);
		SetHighColor(0, 0, 0);
		FillRect(updateRect);
		FillRect(rect);
	}
}


