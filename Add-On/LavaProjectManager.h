/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAPROJECTLPM_H
#define LAVAPROJECTLPM_H

#include <String.h>
#include <List.h>
#include <Directory.h>
#include <File.h>
#include <Path.h>

#include "LavaProject.h"
#include "LavaFilePanel.h"
#include "ProjectTypeSelector.h"
#include "Exceptions/ProjectManagerException.h"

class LavaProjectManager
{
	public:
		LavaProjectManager();
		~LavaProjectManager();
		void CreateNewProject(int DiscType);
		LavaProject* OpenPorject();
		LavaProject* OpenPorject(BString Porject);
		LavaProject* AddToPorject(FileTree *FileStructure, int DiscType);
		LavaProject* AddToPorject(FileTree *FileStructure);
		void setStateContainer(BString *StateCont);
		BString getActProjectPath();
		void createProjectFromAbstractProject(BString Path);
			
	private:
		BString strPathToProject;
		BString fActProject;
		LavaProject *fLavaProject;
		BDirectory *fObjDir;
		BFile *fObjFile;
		BNode *fNode;
		char buffer[500];
		char buffer2[500];
		
		BString *fFileName;
		BString *fPathAndFile;
		BString fAlertTMP;
		BEntry fEntry;
		BEntry fParentEntry;
		LavaFilePanel *fObjLavaFilePanel;
		BPath *objTempPath;
		BString *fStateContainer;
		int fProjectDiscType;
		
		void _writeToPorject(FileTree *FileStructure);
		void _writeToPorject(BList *nodes, BString *innerProject);
		void _readPorject(BString *Path);
		void _writeProjectFromAbstractProject();
};

#endif
