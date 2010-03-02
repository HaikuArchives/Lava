/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAPROJECTFT_H
#define LAVAPROJECTFT_H

#include <String.h>
#include <Message.h>
#include <List.h>
#include <stdio.h>
#include <Entry.h>

class FileTree {
	public:
		bool File;
		bool Query;
		BString *Path;
		int64 *intSize;
		BList *Nodes;
		
		/*FileTree::*/FileTree();
		FileTree(BString *pPath, bool pFile, int64 *pintSize, bool pQuery);
		~FileTree();
		
		void AddNode(BString Parent, BString Path, bool File, int64 *intSize, bool Query);
		void printNodes();
		void printNodes(BList *nodes);
		FileTree* getNode(BList *nodes, BString *Path);
		void AddTreeToBMsg(BMessage *objMsg);
		void AddFisrtInstanceAsRefToBMsg(BMessage *objMsg);
	
	private:
		void _addTreeToBMsg(BList *nodes);
		BMessage *fObjMsg;
};

#endif
