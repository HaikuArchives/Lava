/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
 */

#ifndef IOSETTINGS_H
#define IOSETTINGS_H

#include <String.h>
#include <File.h>
#include <Path.h>
#include <Node.h>
#include <FindDirectory.h>
#include <fs_attr.h>

#include "../Exceptions/IOSettingsException.h"

class IOSettings {
public:
	IOSettings(BString SettingsName);
	virtual ~IOSettings();
	
	virtual void Init() {};
	void *ReadSetting(const char* Setting);
	void WriteSetting(const char* Setting, const char* SettCont);
	void WriteSetting(const char* Setting, BString SettCont);
	void WriteSetting(const char* Setting, int32 SettCont);
	void WriteSetting(const char* Setting, int64 SettCont);
	void WriteSetting(const char* Setting, double SettCont);
	void WriteSetting(const char* Setting, bool SettCont);

protected:
	BString fSettingsFileName;
	BPath fPath;
	BString fSettingsFile;

private:
	void CreateSettingsFile();
	bool SettingsFileExists();
};

#endif