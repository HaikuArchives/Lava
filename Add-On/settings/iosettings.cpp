/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
 */

#include "iosettings.h"

IOSettings::IOSettings(BString SettingsName)
: fSettingsFileName(SettingsName)
{
	if(find_directory(B_USER_SETTINGS_DIRECTORY, &fPath) != B_OK)
		throw new IOSettingsException(new BString("can't find system settings folder"));
	
	fSettingsFile = fPath.Path();
	fSettingsFile += "/";
	fSettingsFile += fSettingsFileName;
	
	if(!SettingsFileExists())
		CreateSettingsFile();
}


IOSettings::~IOSettings()
{
}


/*	ReadSetting
*	this method can read attributes from a File, just give the name of the
*	attribute to the method (const char) and it returns a pointer to the content of the attribute.
*	It's importent that you know the type of the content because the method returns
* 	a void pointer and you have to make a typ caste to the return value to the expected
*	type of the attribute.
*	If there are errors the method throws exceptions
*/
void
*IOSettings::ReadSetting(const char* Setting)
{
	if(!SettingsFileExists())
		CreateSettingsFile();

	BNode objNode;
	char buffer[500];
	attr_info info;
	memset(buffer, 0, sizeof(buffer));

	//content pointer
	BString *strCont;
	int32 *int32Cont;
	int64 *int64Cont;
	double *dbCont;
	bool *blCont;
	
	objNode.SetTo(fSettingsFile.String());
	objNode.GetAttrInfo(Setting, &info);	
	switch(info.type)
	{
		case B_STRING_TYPE:
		{
			if(objNode.ReadAttr(Setting, info.type, 0, buffer, sizeof(buffer)) == B_ENTRY_NOT_FOUND)
				throw new IOSettingsException(new BString("an Attr. doesn't exsist by some folder (LavaProjectManager::_readPorject)"));
			
			strCont = new BString(buffer);
			return strCont;
		}
		break;
		case B_INT32_TYPE:
		{
			int32Cont = new int32();
			if(objNode.ReadAttr(Setting, info.type, 0, int32Cont, sizeof(int32Cont)) == B_ENTRY_NOT_FOUND)
				throw new IOSettingsException(new BString("an Attr. doesn't exsist by some folder (LavaProjectManager::_readPorject)"));
				
			return int32Cont;
		}
		break;
		case B_INT64_TYPE:
		{
			int64Cont = new int64();
			if(objNode.ReadAttr(Setting, info.type, 0, int64Cont, sizeof(int64Cont)) == B_ENTRY_NOT_FOUND)
				throw new IOSettingsException(new BString("an Attr. doesn't exsist by some folder (LavaProjectManager::_readPorject)"));
			
			return int64Cont;
		}
		break;
		case B_DOUBLE_TYPE:
		{
			dbCont = new double();
			if(objNode.ReadAttr(Setting, info.type, 0, dbCont, sizeof(dbCont)) == B_ENTRY_NOT_FOUND)
				throw new IOSettingsException(new BString("an Attr. doesn't exsist by some folder (LavaProjectManager::_readPorject)"));

			return dbCont;
		}
		break;
		case B_BOOL_TYPE:
		{
			blCont = new bool();
			if(objNode.ReadAttr(Setting, info.type, 0, blCont, sizeof(blCont)) == B_ENTRY_NOT_FOUND)
				throw new IOSettingsException(new BString("an Attr. doesn't exsist by some folder (LavaProjectManager::_readPorject)"));

			return blCont;
		}
		break;
		default:
			throw new IOSettingsException(new BString("not supported attribute type was read"));
		break;
	}
}


/*	WriteSetting
*	this method writes a setting to the settings file (strings(const char))
*/
void IOSettings::WriteSetting(const char* Setting, const char* SettCont)
{
	BString tmp;
	tmp.SetTo(SettCont);

	WriteSetting(Setting, tmp);
}


/*	WriteSetting
*	this method writes a setting to the settings file (strings(BString))
*/
void IOSettings::WriteSetting(const char* Setting, BString SettCont)
{
	if(!SettingsFileExists())
		CreateSettingsFile();

	BNode objNode;
	objNode.SetTo(fSettingsFile.String());
	
	if(objNode.WriteAttr(Setting, B_STRING_TYPE, 0, SettCont.String(), SettCont.Length()) == B_FILE_ERROR)
		throw new IOSettingsException(new BString("can't write attr. to settings file (IOSettings::WriteSetting(const char* Setting, BString SettCont))"));

}


/*	WriteSetting
*	this method writes a setting to the settings file (int32)
*/
void IOSettings::WriteSetting(const char* Setting, int32 SettCont)
{
	if(!SettingsFileExists())
		CreateSettingsFile();

	BNode objNode;
	objNode.SetTo(fSettingsFile.String());
	
	if(objNode.WriteAttr(Setting, B_INT32_TYPE, 0, (void*)&SettCont, sizeof(SettCont)) == B_FILE_ERROR)
		throw new IOSettingsException(new BString("can't write attr. to settings file (IOSettings::WriteSetting(const char* Setting, int32 SettCont))"));

}


/*	WriteSetting
*	this method writes a setting to the settings file (int64)
*/
void IOSettings::WriteSetting(const char* Setting, int64 SettCont)
{
	if(!SettingsFileExists())
		CreateSettingsFile();

	BNode objNode;
	objNode.SetTo(fSettingsFile.String());
	
	if(objNode.WriteAttr(Setting, B_INT64_TYPE, 0, (void*)&SettCont, sizeof(SettCont)) == B_FILE_ERROR)
		throw new IOSettingsException(new BString("can't write attr. to settings file (IOSettings::WriteSetting(const char* Setting, int64 SettCont))"));

}


/*	WriteSetting
*	this method writes a setting to the settings file (double)
*/
void IOSettings::WriteSetting(const char* Setting, double SettCont)
{
	if(!SettingsFileExists())
		CreateSettingsFile();

	BNode objNode;
	objNode.SetTo(fSettingsFile.String());
	
	if(objNode.WriteAttr(Setting, B_DOUBLE_TYPE, 0, (void*)&SettCont, sizeof(SettCont)) == B_FILE_ERROR)
		throw new IOSettingsException(new BString("can't write attr. to settings file (IOSettings::WriteSetting(const char* Setting, double SettCont))"));

}


/*	WriteSetting
*	this method writes a setting to the settings file (boolean)
*/
void IOSettings::WriteSetting(const char* Setting, bool SettCont)
{
	if(!SettingsFileExists())
		CreateSettingsFile();

	BNode objNode;
	objNode.SetTo(fSettingsFile.String());
	
	if(objNode.WriteAttr(Setting, B_BOOL_TYPE, 0, (void*)&SettCont, sizeof(SettCont)) == B_FILE_ERROR)
		throw new IOSettingsException(new BString("can't write attr. to settings file (IOSettings::WriteSetting(const char* Setting, bool SettCont))"));

}


/*	SettingsFileExists
*	this method checks if a file exists, if yes return true, if not return false
*/
bool IOSettings::SettingsFileExists()
{
	BFile objFile;
	
	if(objFile.SetTo(fSettingsFile.String(), B_READ_ONLY | B_FAIL_IF_EXISTS) == B_OK)
		return true;
	else
		return false;
}


/*	CreateSettingsFile
*	this method creates a settings file in the system settings folder
*/
void IOSettings::CreateSettingsFile()
{
	BFile objFile;
	objFile.SetTo(fSettingsFile.String(), B_READ_ONLY | B_CREATE_FILE | B_FAIL_IF_EXISTS);
}
