/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
 */
#include <stdlib.h>
#include <File.h>

#include "BurnDevice.h"

BurnDevice::BurnDevice(bool hasDebugOutput, BHandler *debugHandler, BHandler *errorHandler)
{
	fDAO = " -dao";
	fBurnProof = "";
	fIgnoreSize = "";
	fBurnSpeed = 1;
	fFifoBuffer = 4;
	fEjectWhenFinished = " -eject";
	fSimulation = "";
	fHasDebugOutput = hasDebugOutput;
	fDev = NULL;

	fErrorHandler = errorHandler;
	fLogHandler = debugHandler;
	
	if(fErrorHandler != NULL)
		fErrorMessenger = new BMessenger(fErrorHandler);
		
	if(fLogHandler != NULL)
		fLogMessenger = new BMessenger(fLogHandler);

	fCDRecordInfo = CallCDRecord("-version");
	BString tmpDevices = CallCDRecord("-scanbus | grep \"Removable CD-ROM\"");

	fDevices = new BList();
	FindAllDevices(tmpDevices);
}

BurnDevice::~BurnDevice()
{
	while(!fDevices->IsEmpty()) {
		int i = 0;
		burn_device *item = (burn_device*)fDevices->RemoveItem(i);
		delete item;
	}

	delete fDevices;
}


BList* BurnDevice::GetBurnDevices()
{
	return fDevices;
}

void BurnDevice::SetBurnDevice(burn_device *dev)
{
	fDev = dev;
}

burn_device* BurnDevice::GetCurrentBurnDevice()
{
	return fDev;
}

void BurnDevice::SetDao(bool dao)
{	
	if(dao) {
		fDAO = " -dao";
		SendDebugMsg("BurnDevice seted to dao");
	}
	else {
		fDAO = " -tao";
		SendDebugMsg("BurnDevice seted to tao");
	}
}

void BurnDevice::SetBurnProof(bool burnproof)
{
	if(burnproof) {
		fBurnProof = " driveropts=burnfree";
		SendDebugMsg("burnproof enabled for BurnDevice");
	}
	else {
		fBurnProof = "";
		SendDebugMsg("burnproof disabled for BurnDevice");
	}
}

void BurnDevice::SetIgnoreSize(bool ignoreSize)
{
	if(ignoreSize) {
		fIgnoreSize = " -overburn";
		SendDebugMsg("overburn enabled for BurnDevice");
	}
	else {
		fIgnoreSize = "";
		SendDebugMsg("overburn disabled for BurnDevice");
	}
}

void BurnDevice::SetBurnSpeed(int32 burnspeed)
{
	fBurnSpeed = burnspeed;
	
	BString tmp;
	tmp <<"burn speed seted to " <<fBurnSpeed;
	SendDebugMsg(tmp);
}

void BurnDevice::SetFifoBuffer(int32 fifoBuffer)
{
	fFifoBuffer = fifoBuffer;
	
	BString tmp;
	tmp <<"FIFO buffer seted to " <<fifoBuffer;
	SendDebugMsg(tmp);
}

void BurnDevice::SetEjectWhenFinished(bool eject)
{
	if(eject) {
		fEjectWhenFinished = " -eject";
		SendDebugMsg("eject after burning enabled");
	}
	else {
		fEjectWhenFinished = "";
		SendDebugMsg("eject after burning disabled");
	}
}

void BurnDevice::SetSimulation(bool simulation)
{
	if(simulation) {
		fSimulation = " -dummy";
		SendDebugMsg("simulate befor burning");
	}
	else {
		fSimulation = "";
		SendDebugMsg("don't simulate befor burning");
	}
}


void BurnDevice::BurnCUEFile(BEntry *cueFile)
{
}

void BurnDevice::BurnISOImage(BEntry *isoFile)
{
	// check device
	if(!fDev)  {
		SendErrorMsg("Invalid device");
		return;
	}

	// check file
	if(isoFile->InitCheck() != B_OK) {
		SendErrorMsg("Error reading file");
		return;
	}

	// check disc
	bd_disc_state disc = GetDiscInfo();
	if(disc != BD_EMPTY_DISC) {
		switch(disc) {
			case BD_DISC_ERROR:
				SendErrorMsg("Error reading disc");
				break;
			case BD_NO_DISC:
				SendErrorMsg("No disc");
				break;
			case BD_MULTISESSION_DISC:
				SendErrorMsg("Multisession disc full");
				break;
			case BD_FULL_DISC:
				SendErrorMsg("Disc full");
				break;
			default:
				break;
		}
		return;
	}

	// burn
	char name[B_FILE_NAME_LENGTH]; 
	isoFile->GetName(name); 

	BString param = "";
	param << "-v -pad -data " << fSimulation << fDAO << fIgnoreSize << fFifoBuffer << fBurnProof << fEjectWhenFinished << fBurnSpeed << fDev->deviceNumber << name; // Musess + fixate + bootimage

	CallCDRecord(param, true);
	// fCDRecordInfo = CallCDRecord("-version");
	// BString tmpDevices = CallCDRecord("-scanbus | grep \"Removable CD-ROM\"");
}

BEntry* BurnDevice::CopyDisc(burn_device *source, burn_device *target, bool isAudioDisc, bool saveIso)
{
}

BEntry* BurnDevice::CreateISOFromDisc()
{
}


void BurnDevice::BlankDisc(burn_device *dev)
{
}

void BurnDevice::FixateDisc(burn_device *dev)
{
}

bd_disc_state BurnDevice::GetDiscInfo()
{
	bd_disc_state t = BD_DISC_ERROR;

	if(fDev) {
		BString param = "";
		param << "dev=" << fDev->deviceNumber << " -toc";
		BString discInfo = CallCDRecord(param);

		if(discInfo.FindFirst("No disk") != B_ERROR)
			t = BD_NO_DISC;
		else if(discInfo.FindFirst("track") != B_ERROR) {
			// multisession missing!!
			t = BD_FULL_DISC;
		}
		else
			t = BD_EMPTY_DISC;
	}
	
	return t;
}


void BurnDevice::SetStatusBar(BStatusBar *statusBar)
{
	fStatusBar = statusBar;
}

void BurnDevice::SetErrorHandler(BHandler *handler)
{
	fErrorHandler = handler;
}

void BurnDevice::SetDebugHandler(BHandler *handler)
{
	fLogHandler = handler;
}

void BurnDevice::SetDebugOutput(bool hasDebugOutput)
{
	fHasDebugOutput = hasDebugOutput;
}


BString BurnDevice::GetCDRecordInfo()
{
	return fCDRecordInfo;
}


BString BurnDevice::CallCDRecord(BString param, bool callInBackground)
{
	BString cdrecord, logFileName, retString;
	logFileName << "/tmp/BurnDevice.log";
	cdrecord << "cdrecord " << param << " > " << logFileName << " 2>&1";

	if(callInBackground)
		cdrecord << " &";

	SendDebugMsg(cdrecord);
	int32 ret = system(cdrecord.String());

	if(callInBackground)
		return "";

	BEntry entry(logFileName.String());
	
	if(entry.Exists()) {	
		BFile logFile(&entry, B_READ_ONLY);
		off_t size;
		ssize_t len;
		logFile.GetSize(&size);
		char buffer[size];

		len = logFile.Read((void *)buffer, size);
		
		if(len < 0)
			SendErrorMsg("Error while parsing logfile!");
		else
		 	buffer[len-1] = '\0';

		retString << buffer;

		SendDebugMsg(retString);
		// entry.Remove();
	}
	else {
		SendErrorMsg("Error while creating logfile!");
	}

	return retString;
}

BString BurnDevice::CallMkISOFs(BString param)
{
}


void BurnDevice::SendDebugMsg(BString debugMsg)
{
	if(fHasDebugOutput) {
		if(fLogHandler) {
			BMessage *msg = new BMessage('log');
			msg->AddString("DebugMsg", debugMsg);
			fLogMessenger->SendMessage(msg);
			delete msg;
		}
		else {
			printf("%s", debugMsg.String());
		}
	}
}

void BurnDevice::SendErrorMsg(BString errorMsg)
{
	if(fErrorHandler) {
			BMessage *msg = new BMessage('log');
			msg->AddString("ErrorMsg", errorMsg);
			fErrorMessenger->SendMessage(msg);
			delete msg;
	}
	else {
		printf("%s", errorMsg.String());
	}
}

void BurnDevice::FindAllDevices(BString source)
{
	int32 count = 0;
	BString tmp;
	bool endOfField = false;
	
	for(int32 i = 0; i < source.Length(); i++) {
		if(source[i] == 10) {
			endOfField = false;
			count = 0;
			tmp = "";
			continue;
		}

		if(source[i] != ' ' && source[i] != '\t') {
			if(source[i] == '\'')
				while(source[++i] != '\'')
					tmp += source[i];
			else
				tmp += source[i];
			endOfField = true;
		}
		else {
			if(endOfField) {
				switch(count) {
					case 0: {
						burn_device *newDev = new burn_device();
						newDev->deviceNumber = tmp;
						fDevices->AddItem((void*)newDev);
						break;
					}
					case 2: {
						burn_device *newDev = (burn_device*)fDevices->LastItem();
						newDev->vendorID = tmp;
						break;
					}
					case 3: {
						burn_device *newDev = (burn_device*)fDevices->LastItem();
						newDev->name = tmp;
						break;
					}
					case 4: {
						burn_device *newDev = (burn_device*)fDevices->LastItem();
						newDev->additionalInfo = tmp;
						break;
					}
				}
				endOfField = false;
				tmp = "";
				count++;
			}
		}
	}
}

