#include "LavaMainException.h"

LavaMainException::LavaMainException(BString *getText)
: Exception(getText)
{
}


LavaMainException::LavaMainException(BString *getText, void* getData)
: Exception(getText, NULL, getData)
{
}


LavaMainException::~LavaMainException()
{
}