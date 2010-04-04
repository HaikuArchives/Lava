/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#include "LavaProject.h"
#include <Alert.h>

LavaProject::LavaProject(BString Project)
: fProject(new BString(Project)), ProjectStructure(new FileTree()), ProjectSize(0),
 DiscType(-1)
{
}

LavaProject::LavaProject(BMessage* archive)
: fProject(new BString())
{	
	BMessage objMessage;
	if(archive->FindMessage("ProjectStructure", &objMessage) == B_OK) {
		ProjectStructure = reinterpret_cast<FileTree*>(FileTree::Instantiate(&objMessage));
	}
		
	archive->FindInt64("ProjectSize", ProjectSize);
	archive->FindInt32("DiscType", DiscType);
	archive->FindString("fProject", fProject);
}

LavaProject::~LavaProject()
{
	delete ProjectStructure, fProject;
}


/*	getProjectName
*	returns the name of the project
*/
BString*
LavaProject::getProjectName()
{
	return fProject;
}


/*	Instantiate
*	rebuild archived object from BMessage
*/
BArchivable*
LavaProject::Instantiate(BMessage* archive)
{
	if(validate_instantiation(archive, "LavaProject")) {
		return new LavaProject(archive); 
	}
	
	return NULL; 
}


/*	Archive
	method build an archive of this object and save it to the given BMessage
*/
status_t
LavaProject::Archive(BMessage* archive, bool deep) const
{
	try {
		archive->AddString("class", "LavaProject");
		
		if(deep) {
			BMessage objMessage;
	 		
	 		if(ProjectStructure->Archive(&objMessage, deep) == B_OK)
	 			archive->AddMessage("ProjectStructure", &objMessage);
		}
		
		archive->AddInt64("ProjectSize", ProjectSize);
		archive->AddInt32("DiscType", DiscType);
		archive->AddString("fProject", fProject->String());
		
		return B_OK;
	}
	catch(...) {
		return B_ERROR;
	}
}
