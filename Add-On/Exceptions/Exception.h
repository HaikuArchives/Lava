/*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
*/

#ifndef LAVAEXCEPTION_H
#define LAVAEXCEPTION_H

#include <String.h>
//#include <map.h>
#include <map>

class Exception {
	public:
		int intErrorCode;
		BString *ExceptionText;
		std::map<BString, void*> Data;
		
		Exception(BString *getText);
		Exception(BString *getText, void* getThrowedException);
		Exception(BString *getText, void* getThrowedException, void* getData);
		virtual ~Exception();
};

#endif
