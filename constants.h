#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>

#define LUNAR_DEBUG
#define WINDOWS
#ifdef LUNAR_DEBUG
#define SOFTWARE_VERSION QString("1.0 - Fiery Moon Moon - Beta Debug")
#else
#define SOFTWARE_VERSION QString("1.0 - Fiery Moon")
#endif
#define CONFIGURATION_FILE "data/config.ini"
#define PLATFORM_FILE "data/platforms.txt"
#define CACHE_PATH "cache/"

#define STANDARD_FR_DATETIME QString("dd/MM/yyyy")

#endif // CONSTANTS_H
