/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAEXCEPTIONPM_H
#define LAVAEXCEPTIONPM_H

#include <Directory.h>
#include <File.h>
#include <Node.h>
#include <Entry.h>

#include "Exception.h"
#include "LavaFilePanel.h"

class ProjectManagerException : public Exception {
	public:
		ProjectManagerException(BString *getText);
		ProjectManagerException(BString *getText, BDirectory *objDir);
		ProjectManagerException(BString *getText, BNode *objNode);
		ProjectManagerException(BString *getText, BDirectory *objDir, BNode *objNode);
		ProjectManagerException(BString *getText, LavaFilePanel *objLavaFilePanel);
		ProjectManagerException(BString *getText, LavaFilePanel *objLavaFilePanel, BDirectory *objDir);
		ProjectManagerException(BString *getText, LavaFilePanel *objLavaFilePanel, BNode *objNode);
		ProjectManagerException(BString *getText, BDirectory *objDir, BNode *objNode, BFile *objFile, BEntry *objEntry);
		~ProjectManagerException();
};

#endif