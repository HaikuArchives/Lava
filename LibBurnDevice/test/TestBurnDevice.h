/*
 *  TestBurnDevice is a simple GUI to check and explain the BurnDevice class
 *  It is also a (very ugly) CD burner, use it to write a better one :)
 *
 *  2007 by Team Maui, jan__64
 */

#ifndef TESTBURNDEVICE_H
#define TESTBURNDEVICE_H


#include <iostream> //gcc4
//#include <iostream.h> //gcc2
#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <List.h>
#include <ListItem.h>
#include <ListView.h>
#include <StringView.h>
#include <Window.h>

#include "../BurnDevice.h"

const uint32 CHECKBUTTON = 'TBcb'; 
const uint32 DEVICELIST = 'TBdl'; 


// The test application
class TestBurnDevice : public BApplication
{
public:
	TestBurnDevice();
	~TestBurnDevice()
	{
		delete burner;
	}
	BurnDevice *burner;
	BList *devList;
	BStringView *information;
};



// the window that receives messages
class TestBurnWindow : public BWindow
{
public:
	TestBurnWindow(BRect r, const char* name, window_type w, uint32 f)
		: BWindow(r, name, w, f)
	{}
	void MessageReceived(BMessage *msg);
	bool QuitRequested()
	{
		be_app_messenger.SendMessage(B_QUIT_REQUESTED); 
		return true;
	}
};

#endif
