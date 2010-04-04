/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
 */

#ifndef BURNDEVICE__H
#define BURNDEVICE__H

#include <Entry.h>
#include <List.h>
#include <Handler.h>
#include <StatusBar.h>
#include <String.h>
#include <Message.h>
#include <Messenger.h>
#include <Alert.h>
#include <stdio.h>

struct burn_device {
	BString deviceNumber, vendorID, name, additionalInfo;
};


/*
enum bd_error
{
	BD_LOGFILE_ERROR,		// cannot create logfile
	BD_PARSE_ERROR,			// error parsing logfile
	BD_CDRECORD_ERROR,		// cdrecord missing or invalid
	BD_MKISOFS_ERROR,		// mkisofs missing or invalid
	BD_INVALID_DEVICE,		// device is NULL or invalid
	BD_TIME_OUT, 			// time out after 10 seconds while waiting to start burning
	BD_BURN_ERROR,			// error while burning
	BD_IMGFILE_ERROR,		// image file could not be saved
	BD_BLANK_ERROR,			// error while blanking
	BD_FIXATE_ERROR			// error while fixating
};
*/

enum bd_filesystem {
	BD_CDDA,			// audio CDs
	BD_WINDOWS_JOLIET,		// default for data CDs
	BD_ISO9660_31,
	BD_ISO9660_8_3,
	BD_UDF_DATA_DVD,		// default for data DVDs
	BD_UDF_VIDEO_DVD,
	BD_VCD_1_1,			// default for video disks
	BD_VCD_2_0,
	BD_SVCD
};

enum bd_disc_state {
	BD_DISC_ERROR,			// disc could not be read
	BD_NO_DISC,			// no disc
	BD_EMPTY_DISC,			// empty disc
	BD_MULTISESSION_DISC,		// multisession disc
	BD_FULL_DISC			// disc is not empty
};


class BurnDevice {
	public:
		BurnDevice(bool hasDebugOutput = true, BHandler *debugHandler = NULL, BHandler *errorHandler = NULL);
		~BurnDevice();

		BList* GetBurnDevices();
		void SetBurnDevice(burn_device *dev);
		burn_device* GetCurrentBurnDevice();

		void SetDao(bool dao);
		void SetBurnProof(bool burnproof);
		void SetIgnoreSize(bool ignoreSize);
		void SetBurnSpeed(int32 burnspeed);
		void SetFifoBuffer(int32 fifoBuffer);
		void SetEjectWhenFinished(bool eject);
		void SetSimulation(bool simulation);

		void BurnAudioCD(BList *songList, bool hasCopyProtection = false);
		void BurnDataCD(BList *fileList, bool multiSession = false, BEntry *bootImage = NULL);
		void BurnDataDVD(BList *fileList, bool multiSession = false, BEntry *bootImage = NULL);
		void BurnVideoDisc(BList *fileList, bool hasHighQuality = false, bool hasPAL = true);
		void BurnCUEFile(BEntry *cueFile);
		void BurnISOImage(BEntry *isoFile);
		BEntry* CopyDisc(burn_device *source, burn_device *target = NULL, bool isAudioDisc = false, bool saveISO = false);
		BEntry* CreateISOFromDisc();
		BEntry* CreateISOFromFiles(BList *fileList);

		void BlankDisc(burn_device *dev = NULL);
		void FixateDisc(burn_device *dev = NULL);
		bd_disc_state GetDiscInfo();

		void SetStatusBar(BStatusBar *statusbar);
		void SetErrorHandler(BHandler *handler);
		void SetDebugHandler(BHandler *handler);
		void SetDebugOutput(bool hasDebugOutput);

		BString GetCDRecordInfo();
		BString GetMkISOFsInfo();

		BString CallCDRecord(BString param, bool callInBackground = false);
		BString CallMkISOFs(BString param);

	private:
		void SendErrorMsg(BString errorMsg);
		void SendDebugMsg(BString debugMsg);
		void FindAllDevices(BString source);

		burn_device *fDev;
		BString fDAO, fBurnProof, fIgnoreSize, fEjectWhenFinished, fSimulation;
		int32 fBurnSpeed, fFifoBuffer;
		
		BMessenger *fErrorMessenger, *fLogMessenger;
		
		BHandler *fErrorHandler, *fLogHandler;
		BStatusBar *fStatusBar;

		bool fHasDebugOutput;
		BString fCDRecordInfo, fMkIsoFsInfo, fLogFile;

		BList *fDevices;
};

#endif

