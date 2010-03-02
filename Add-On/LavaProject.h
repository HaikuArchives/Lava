/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAPROJECT_H
#define LAVAPROJECT_H

#include <String.h>

#include "FileTree.h"

class LavaProject {
	public:
		BString* getProjectName();
		LavaProject(BString PathToProject);
		~LavaProject();
		
		FileTree *ProjectStructure;
		off_t ProjectSize;
		int DiscType;
	
	private:
		BString *fProject;
};

#endif
