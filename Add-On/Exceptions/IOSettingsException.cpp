#include "IOSettingsException.h"

IOSettingsException::IOSettingsException(BString *getText)
: Exception(getText)
{
}


IOSettingsException::~IOSettingsException()
{
}