/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAEXCEPTIONLME_H
#define LAVAEXCEPTIONLME_H

#include "Exception.h"

class LavaMainException : public Exception {
	public:	
		LavaMainException(BString *getText);
		LavaMainException(BString *getText, void* getData);
		~LavaMainException();
};

#endif