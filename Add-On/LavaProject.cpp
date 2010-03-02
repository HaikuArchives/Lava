#include "LavaProject.h"

LavaProject::LavaProject(BString Project)
: fProject(new BString(Project)), ProjectStructure(new FileTree()), ProjectSize(0),
 DiscType(-1)
{
}


LavaProject::~LavaProject()
{
	delete ProjectStructure, fProject;
}


BString*
LavaProject::getProjectName()
{
	return fProject;
}