#include "logger.hpp"

Logger& Log()
{
    static Logger theLog;
    return theLog;
}

