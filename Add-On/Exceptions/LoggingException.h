/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAEXCEPTIONLOG_H
#define LAVAEXCEPTIONLOG_H

#include <Node.h>

#include "Exception.h"

class LoggingException : public Exception {
	public:	
		LoggingException(BString *getText);
		~LoggingException();
};

#endif