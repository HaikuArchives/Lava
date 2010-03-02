#include "LavaSettings.h"

LavaSettings::LavaSettings()
: IOSettings("Lava")
{
	Init();
}


LavaSettings::~LavaSettings()
{
}


/*	Init
*	initializing Lava settings file
*/
void
LavaSettings::Init()
{
	char buffer[500];
	BNode objNode;
	memset(buffer, 0, sizeof(buffer));
	objNode.SetTo(fSettingsFile.String());
	
	//inits the actproject setting
	if(objNode.ReadAttr("LAVA:ActProject", B_STRING_TYPE, 0, buffer, sizeof(buffer)) == B_ENTRY_NOT_FOUND)
		if(objNode.WriteAttr("LAVA:ActProject", B_STRING_TYPE, 0, " ", sizeof(" ")) == B_FILE_ERROR)
			throw new IOSettingsException(new BString("can't initialize settings file"));
}