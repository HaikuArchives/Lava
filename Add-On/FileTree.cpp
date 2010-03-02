#include "FileTree.h"
#include <Alert.h>

FileTree::FileTree()
{
	Nodes = new BList();
}


FileTree::FileTree(BString *pPath, bool pFile, int64 *pintSize, bool pQuery)
: Path(pPath), intSize(pintSize)
{
	Nodes = new BList();
	File = pFile;
	Query = pQuery;
}


FileTree::~FileTree()
{
	delete Path, Nodes, intSize;
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
		(new BAlert("", tmpTree->Path->String(), "Exit"))->Go();
		
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
		fObjMsg->AddInt64(tmpTree->Path->String(), *tmpTree->intSize);
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


void
FileTree::AddFisrtInstanceAsRefToBMsg(BMessage *objMsg)
{
	FileTree *tmpTree;
	BEntry *objEntry = new BEntry();
	entry_ref *file_ref = new entry_ref;

	for(int i = 0; i < Nodes->CountItems(); i++) {
		tmpTree = (FileTree*)Nodes->ItemAt(i);
		objEntry->SetTo(tmpTree->Path->String());
		objEntry->GetRef(file_ref);		
		objMsg->AddRef("refs", file_ref);
	}
	
	delete objEntry;
}


/*	getNode
*	searchs for a FileTree object with content x
*	and returns reference of an FileTree object
*	method gets a reference of BList with FileTree objec and a path
*/
FileTree*
FileTree::getNode(BList *nodes, BString *Path)
{
	FileTree *tmpTree = NULL;
	
	for(int i = 0; i < nodes->CountItems(); i++) {
		tmpTree = (FileTree*)nodes->ItemAt(i);
				
		if(*tmpTree->Path == *Path)
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
		Nodes->AddItem(new FileTree(new BString(Path.String()), File, intSize, Query));
	}
	else {
		tmpTree->Nodes->AddItem(new FileTree(new BString(Path.String()), File, intSize, Query));
	}
}
