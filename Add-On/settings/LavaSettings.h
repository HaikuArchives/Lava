 /*
 * Copyright 2007 Team MAUI All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Robert Stiehler, Negr0@team-maui.org
 */

#ifndef LAVASETTINGS_H
#define LAVASETTINGS_H

#include "iosettings.h"

class LavaSettings : public IOSettings {
public:
	LavaSettings();
	virtual ~LavaSettings();
	
	virtual void Init();
};

#endif