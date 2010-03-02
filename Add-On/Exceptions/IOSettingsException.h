/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAEXCEPTIONIOS_H
#define LAVAEXCEPTIONIOS_H

#include <Node.h>

#include "Exception.h"

class IOSettingsException : public Exception {
	public:	
		IOSettingsException(BString *getText);
		~IOSettingsException();
};

#endif