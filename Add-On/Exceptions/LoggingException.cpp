#include "LoggingException.h"

LoggingException::LoggingException(BString *getText)
: Exception(getText)
{
}


LoggingException::~LoggingException()
{
}