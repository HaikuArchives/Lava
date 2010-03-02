#include "Exception.h"

Exception::Exception(BString *getText)
: ExceptionText(getText)
{
}


Exception::Exception(BString *getText, void* getThrowedException)
: ExceptionText(getText)
{
	Data["ThrowedException"] = getThrowedException;
}


Exception::Exception(BString *getText, void* getThrowedException, void* getData)
: ExceptionText(getText)
{
	Data["ThrowedException"] = getThrowedException;
	Data["StdAttachment"] = getData;
}


Exception::~Exception()
{
	if(ExceptionText != NULL)
		delete ExceptionText;
}