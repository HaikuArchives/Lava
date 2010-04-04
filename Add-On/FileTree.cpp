/*
 * Copyright 2010 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#include "FileTree.h"
#include <Alert.h>

FileTree::FileTree()
{
	Nodes = new BList();
}


FileTree::FileTree(BMessage* archive)
: Nodes(new BList()), intSize(new int64(0))
{
	BMessage objMessage;
	int32 nodeIndex = 0;
	while(archive->FindMessage("Nodes", nodeIndex, &objMessage) == B_OK) {
		FileTree *tmp = cast_as(Instantiate(&objMessage), FileTree);
		Nodes->AddItem(tmp);
		
		nodeIndex++;
	}
	
	archive->FindBool("File", File);
	archive->FindBool("Query", Query);
	archive->FindString("Path", &Path);
	archive->FindInt64("intSize", intSize);	
}


FileTree::FileTree(BString pPath, bool pFile, int64 *pintSize, bool pQuery)
: Path(pPath), intSize(pintSize)
{
	Nodes = new BList();
	File = pFile;
	Query = pQuery;
}


FileTree::~FileTree()
{
	delete Nodes, intSize;
}


/*	printNodes
*	little helping method to print tree to console
*/
void
FileTree::printNodes()
{
	printNodes(Nodes);
}


/*	printNodes
*	little helping method to print tree with BAlerts
*/
void
FileTree::printNodes(BList *nodes)
{
	FileTree *tmpTree;
	BString strTMP = "";
	
	if(!nodes->IsEmpty())
		(new BAlert("", "Rekursion", "Exit"))->Go();
	
	for(int i = 0; i < nodes->CountItems(); i++) {
		tmpTree = (FileTree*)nodes->ItemAt(i);
		(new BAlert("", "Schleife", "Exit"))->Go();
		(new BAlert("", tmpTree->Path.String(), "Exit"))->Go();
		
		printNodes(tmpTree->Nodes);
	}
}


/*	_addTreeToBMsg
*	method adds the tree to a BMessage object (in a simple form)
*	gets refernce of BList object which contains a object from FileTree
*/
void
FileTree::_addTreeToBMsg(BList *nodes)
{
	FileTree *tmpTree;

	for(int i = 0; i < nodes->CountItems(); i++) {
		tmpTree = (FileTree*)nodes->ItemAt(i);
		fObjMsg->AddInt64(tmpTree->Path.String(), *tmpTree->intSize);
		_addTreeToBMsg(tmpTree->Nodes);
	}
}


/*	AddTreeToBMsg
*	method adds the tree to a BMessage object (in a simple form)
*	gets refernce of BMessage object
*/
void
FileTree::AddTreeToBMsg(BMessage *objMsg)
{
	fObjMsg = objMsg;
	_addTreeToBMsg(Nodes);
}


/*	AddFisrtInstanceAsRefToBMsg
	method adds refs of the files and folders of the "root" folder
	of the selected files to BMessage
*/
void
FileTree::AddFisrtInstanceAsRefToBMsg(BMessage *objMsg)
{
	FileTree *tmpTree;
	BEntry *objEntry = new BEntry();
	entry_ref *file_ref = new entry_ref;

	for(int i = 0; i < Nodes->CountItems(); i++) {
		tmpTree = (FileTree*)Nodes->ItemAt(i);
		objEntry->SetTo(tmpTree->Path.String());
		objEntry->GetRef(file_ref);		
		objMsg->AddRef("refs", file_ref);
	}
	
	delete objEntry;
}


/*	getNode
*	searchs for a FileTree object and returns reference of an FileTree object
*	method gets a reference of BList with FileTree objec and a path
*/
FileTree*
FileTree::getNode(BList *nodes, BString *Path)
{
	FileTree *tmpTree = NULL;
	
	for(int i = 0; i < nodes->CountItems(); i++) {
		tmpTree = (FileTree*)nodes->ItemAt(i);
				
		if(tmpTree->Path == *Path)
			return tmpTree;
		else
			tmpTree = getNode(tmpTree->Nodes, Path);
	}
	
	return tmpTree;
}


/*	AddNode
*	method adds nodes to tree
*	gets parent (or empty String if to root), content (path as string)
*	boolean (true if a file, false if folder) and the size of the file
*	-1 if a folder
*/
void
FileTree::AddNode(BString Parent, BString Path, bool File, int64 *intSize, bool Query)
{
	FileTree *tmpTree;	
	tmpTree = getNode(Nodes, &Parent);
	
	if(tmpTree == NULL) {
		Nodes->AddItem(new FileTree(Path, File, intSize, Query));
	}
	else {
		tmpTree->Nodes->AddItem(new FileTree(Path, File, intSize, Query));
	}
}


/*	Instantiate
*	rebuild archived object from BMessage
*/
BArchivable*
FileTree::Instantiate(BMessage* archive)
{
	if(validate_instantiation(archive, "FileTree")) {
		return new FileTree(archive);
	}
	
	return NULL; 
}


/*	Archive
	method build an archive of this object and save it to the given BMessage
*/
status_t
FileTree::Archive(BMessage* archive, bool deep) const
{
	try {
		archive->AddString("class", "FileTree");
		
		if(deep) {
			FileTree *tmpTree = NULL;
			
			for(int i = 0; i < Nodes->CountItems(); i++) {
				tmpTree = (FileTree*)Nodes->ItemAt(i);
				BMessage objMessage;
				
	 			if(tmpTree->Archive(&objMessage, deep) == B_OK)
	 				archive->AddMessage("Nodes", &objMessage);
			}
		}
		
		archive->AddBool("File", File);
		archive->AddBool("Query", Query);
		archive->AddString("Path", Path.String());
		archive->AddInt64("intSize", (int64)intSize);
	}
	catch(...) {
		return B_ERROR;
	}
}
