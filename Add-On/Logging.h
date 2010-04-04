/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVALOGGING_H
#define LAVALOGGING_H

#include <stdlib.h>
#include <time.h>
#include <String.h>
#include <Directory.h>
#include <File.h>
#include <Node.h>
#include <TypeConstants.h>
#include <FindDirectory.h>
#include <Path.h>

#include "Exceptions/LoggingException.h"

class Logging {
	public:
		Logging();
		~Logging();
		
		void setLogfileName(BString Name);
		void setLogfileDirectory(BString Path);
		void setLogfileDirectory(directory_which Path);
		void generateLogileName(bool generate);
		void generateLogileDirectory(bool generate);
		BString getLogfileName();
		BDirectory getLogfileDirectory();
		void WriteAsAttribute();
		void WriteAsFileContent();
		
		void LogString(BString Content);
		
	protected:
		BString LogfileName;
		BDirectory LogfileDir;
		BFile File;

		void generateLogFile_AND_dir();
		
	private:
		bool LogToAttribute;
		bool generateLogfileName;
		bool generateLogfileDirectory;
};

#endif
