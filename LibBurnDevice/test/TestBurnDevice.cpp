/*
 *  TestBurnDevice is a simple GUI to check and explain the BurnDevice class
 *  It is also a (very ugly) CD burner, use it to write a better one :)
 *
 *  2007 by Team Maui, jan__64
 */

#include "TestBurnDevice.h"


TestBurnDevice::TestBurnDevice()
	: BApplication("application/x-vnd.TestBurnDevice")
{
	// Initialize a new BurnDevice
	burner = new BurnDevice();

	// Retrieve a list of all found burning devices
	devList = burner->GetBurnDevices();

	// create a window with a drawing view and a listview
	TestBurnWindow *win = new TestBurnWindow(BRect(100,100,700,500), "TestBurnDevice", B_TITLED_WINDOW, 0);
	BView *view = new BView(BRect(0,0,600,400), "View", B_FOLLOW_ALL_SIDES, 0);
	view->SetViewColor(216,216,216);
	BListView *list = new BListView(BRect(10,10,200,300), "List");
	list->SetInvocationMessage(new BMessage(DEVICELIST));

	// information text field
	information = new BStringView(BRect(10,310,300,340), "String", "No device selected, double-click to select");

	// go through all devices
	for(int i=0; i<devList->CountItems(); i++)
	{
		// get a device from the list and add the name to the listview
		burn_device *bd = (burn_device*)devList->ItemAt(i);
		list->AddItem(new BStringItem(bd->name.String()));
	}

	// a button to check for a disc
	BButton *checkbutton = new BButton(BRect(210,10,310,30), "disccheck", "Check Disc", new BMessage(CHECKBUTTON));

	// finish the window stuff
	win->AddChild(view);
	view->AddChild(list);
	view->AddChild(checkbutton);
	view->AddChild(information);
	win->Show();
}


void TestBurnWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case DEVICELIST:
		{	
			BListView *list;
			msg->FindPointer("source",(void **) &list);
			int i = list->CurrentSelection();
			burn_device *bd = (burn_device*)((TestBurnDevice*)be_app)->devList->ItemAt(i);
			((TestBurnDevice*)be_app)->information->SetText(bd->name.String());
			((TestBurnDevice*)be_app)->burner->SetBurnDevice(bd);

		}
		break;

		case CHECKBUTTON:
		{
			bd_disc_state t = ((TestBurnDevice*)be_app)->burner->GetDiscInfo();
			switch(t)
			{
				case BD_DISC_ERROR:
					(new BAlert("disc error", "Disc error", "Ok"))->Go();
					break;
				case BD_NO_DISC:
					(new BAlert("no disc", "No disc", "Ok"))->Go();
					break;
				case BD_EMPTY_DISC:
					(new BAlert("empty disc", "Empty disc", "Ok"))->Go();
					break;
				case BD_MULTISESSION_DISC:
					(new BAlert("multisession disc", "Multisession disc", "Ok"))->Go();
					break;
				case BD_FULL_DISC:
					(new BAlert("Full disc", "Full disc", "Ok"))->Go();
					break;
			}
		}
		break;
			
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}

// the main stuff
int main()
{
	TestBurnDevice *t = new TestBurnDevice();
	t->Run();

	return 0;
}

