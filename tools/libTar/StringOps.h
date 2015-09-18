/* 
 * File:   StringOps.h
 * Author: Dante
 *
 * Created on 23 avril 2013, 13:32
 */

#include <iostream>
#include <cstring>

#ifndef STRINGOPS_H
#define	STRINGOPS_H

class StringOps {
public:
    StringOps();
    virtual ~StringOps();
    
    static long long unsigned int octalStringToInt64(const char *s);
    
    static long long unsigned int octalStringToInt64(const char *s, const char **end);
private:

};

#endif	/* STRINGOPS_H */

