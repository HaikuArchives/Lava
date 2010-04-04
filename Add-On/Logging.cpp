/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#include "Logging.h"

Logging::Logging()
{
	generateLogfileName = false;
	generateLogfileDirectory = false;
	LogToAttribute = true;
}


Logging::~Logging()
{
}


void
Logging::setLogfileName(BString Name)
{
	LogfileName = Name;
}


void
Logging::setLogfileDirectory(BString Path)
{
	if(LogfileDir.SetTo(Path.String()) != B_OK)
		throw new LoggingException(new BString("can't find logging folder"));
}


void
Logging::setLogfileDirectory(directory_which dwPath)
{
	BPath Path;

	if(find_directory(dwPath, &Path) != B_OK)
		throw new LoggingException(new BString("can't find logging folder"));
		
	if(LogfileDir.SetTo(Path.Path()) != B_OK)
		throw new LoggingException(new BString("can't find logging folder"));
}


void
Logging::generateLogileName(bool generate)
{
	generateLogfileName = generate;
}


void
Logging::generateLogileDirectory(bool generate)
{
	generateLogfileDirectory = generate;
}


BString
Logging::getLogfileName()
{
	return LogfileName;
}


BDirectory
Logging::getLogfileDirectory()
{
	return LogfileDir;
}


void
Logging::WriteAsAttribute()
{
	LogToAttribute = true;
}


void
Logging::WriteAsFileContent()
{
	LogToAttribute = false;
}


void
Logging::generateLogFile_AND_dir()
{	
	if(generateLogfileDirectory) {
		if(LogfileDir.SetTo("/boot/var/log/") != B_OK) {
			if(LogfileDir.SetTo("/boot/var/") != B_OK) {
				LogfileDir.SetTo("/boot/");
				LogfileDir.CreateDirectory("var", &LogfileDir);
				LogfileDir.SetTo("/boot/var/");
			}
			LogfileDir.CreateDirectory("log", &LogfileDir);
			LogfileDir.SetTo("/boot/var/log/");
		}
		
		generateLogfileDirectory = false; //is now generated and don't need to make it again
	}
	
	if(generateLogfileName) {
		
		do {
			struct tm *tmnow;	
			int randomNumber = 0;
			
			time_t tnow;
			time(&tnow);
			tmnow = localtime(&tnow);
			
			srand((unsigned)time(NULL)); //init random number generator
			randomNumber = rand();
			
			LogfileName <<"BurnDevice logging " <<tmnow->tm_mday <<"-" <<tmnow->tm_mon + 1 <<"-" <<tmnow->tm_year + 1900 <<" " <<randomNumber;
		} while(LogfileDir.CreateFile(LogfileName.String(), &File, true) != B_OK);
		
		generateLogfileName = false; //is now generated and don't need to make it again
	}
	
	if(File.SetTo(&LogfileDir, LogfileName.String(), B_READ_WRITE | B_CREATE_FILE | B_OPEN_AT_END) != B_OK)
		throw new LoggingException(new BString("setting/generating of logfile failed"));
}


void
Logging::LogString(BString Content)
{
	generateLogFile_AND_dir();

	if(!LogToAttribute) {
		Content << "\n";
		if(File.Write(Content.String(), Content.Length()) != B_OK)
			throw new LoggingException(new BString("logging to file failed"));
	}
	else {
		char buffer[500];
		BString LogginContent = "";
		
		memset(buffer, 0, sizeof(buffer));
		File.ReadAttr("Logging", B_STRING_TYPE, 0, buffer, sizeof(buffer));
		LogginContent <<buffer <<"\n" <<Content;
		
		File.WriteAttr("Logging", B_STRING_TYPE, 0, LogginContent.String(), LogginContent.Length());
	}
}
