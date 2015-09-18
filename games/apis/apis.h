#ifndef APIS_H
#define APIS_H

#include <QSettings>
#include "../../constants.h"
#include "abstractapi.h"
#include "jvcomapi.h"

class ApiSelector
{
public:
    static AbstractApi* getUsedApi();
};

#endif // APIS_H
