/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAEXCEPTIONPTSPTSE_H
#define LAVAEXCEPTIONPTSPTSE_H

#include "Exception.h"

class ProjectTypeSelectorException : public Exception {
	public:	
		ProjectTypeSelectorException(BString *getText);
		~ProjectTypeSelectorException();
};

#endif