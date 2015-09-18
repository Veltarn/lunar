/* 
 * File:   StringOps.cpp
 * Author: Dante
 * 
 * Created on 23 avril 2013, 13:32
 */

#include "StringOps.h"

StringOps::StringOps() {
}

StringOps::~StringOps() {
}

long long unsigned int StringOps::octalStringToInt64(const char *s)
{
    const char *end;
    return StringOps::octalStringToInt64(s, &end);
}

long long unsigned int StringOps::octalStringToInt64(const char *s, const char **end)
{
    long long unsigned int result = 0;
    for(;;)
    {
        char c = *s;
        if(c < '0' || c > '7')
        {
            if(end != NULL)
                *end = s;
            return result;
        }
        
        result <<= 3;
        result += (c - '0');
        s++;
    }
}
