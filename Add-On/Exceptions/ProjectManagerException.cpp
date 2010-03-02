#include "ProjectManagerException.h"

ProjectManagerException::ProjectManagerException(BString *getText)
: Exception(getText)
{
}


ProjectManagerException::ProjectManagerException(BString *getText, BDirectory *objDir)
: Exception(getText)
{
	Data["DirAttachment"] = objDir;
}


ProjectManagerException::ProjectManagerException(BString *getText, BNode *objNode)
: Exception(getText)
{
	Data["NodeAttachment"] = objNode;
}


ProjectManagerException::ProjectManagerException(BString *getText, BDirectory *objDir, BNode *objNode)
: Exception(getText)
{
	Data["DirAttachment"] = objDir;
	Data["NodeAttachment"] = objNode;
}


ProjectManagerException::ProjectManagerException(BString *getText, LavaFilePanel *objLavaFilePanel)
: Exception(getText)
{
	Data["LavaFilePanel"] = objLavaFilePanel;
}


ProjectManagerException::ProjectManagerException(BString *getText, LavaFilePanel *objLavaFilePanel, BDirectory *objDir)
: Exception(getText)
{
	Data["DirAttachment"] = objDir;
	Data["LavaFilePanel"] = objLavaFilePanel;
}

ProjectManagerException::ProjectManagerException(BString *getText, LavaFilePanel *objLavaFilePanel, BNode *objNode)
: Exception(getText)
{
	Data["NodeAttachment"] = objNode;
	Data["LavaFilePanel"] = objLavaFilePanel;
}


ProjectManagerException::ProjectManagerException(BString *getText, BDirectory *objDir, BNode *objNode, BFile *objFile, BEntry *objEntry)
: Exception(getText)
{
	Data["DirAttachment"] = objDir;
	Data["NodeAttachment"] = objNode;
	Data["FileAttachment"] = objFile;
	Data["EntryAttachment"] = objEntry;
}


ProjectManagerException::~ProjectManagerException()
{
	//cleanup
	LavaFilePanel *FilePanel = (LavaFilePanel*)Data["LavaFilePanel"];
	if(FilePanel != NULL) {
		FilePanel->Lock();
		FilePanel->Quit();
	}
	
	BDirectory *dir = (BDirectory*)Data["DirAttachment"];
	if(dir != NULL)
		dir->Unset();
		
	BNode *node = (BNode*)Data["NodeAttachment"];
	if(node != NULL)
		node->Unset();
		
	BFile *file = (BFile*)Data["FileAttachment"];
	if(file != NULL)
		file->Unset();
		
	BEntry *entry = (BEntry*)Data["EntryAttachment"];
	if(entry != NULL)
		entry->Unset();
}