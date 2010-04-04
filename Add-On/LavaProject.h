/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAPROJECT_H
#define LAVAPROJECT_H

#include <Archivable.h>
#include <String.h>

#include "FileTree.h"

class LavaProject : public BArchivable {
	public:
		BString* getProjectName();
		LavaProject(BString PathToProject);
		LavaProject(BMessage* archive);
		~LavaProject();
		
		static BArchivable* Instantiate(BMessage* archive);
		virtual status_t Archive(BMessage* archive, bool deep = true) const;
		
		FileTree *ProjectStructure;
		off_t ProjectSize;
		int DiscType;
	
	private:
		BString *fProject;
};

#endif
