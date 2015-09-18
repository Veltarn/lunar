/* 
 * File:   AbstractTar.h
 * Author: Dante
 *
 * Created on 23 avril 2013, 23:14
 */

#ifndef ABSTRACTTAR_H
#define	ABSTRACTTAR_H

#define WINDOWS
#define BLOCK 512

#include <iostream>
#include <vector>
#include <cmath>
#include <exception>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <unistd.h>
#ifdef WINDOWS
#include <windows.h>
#include <Lmcons.h>
#endif
#include "FileInfo.h"
#include "StringOps.h"

struct PosixTarHeader
{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeFlag[1];
    char linkName[100];
    char ustarIndicator[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devMajor[8];
    char devMinor[8];
    char filenamePrefix[155];
    char padding[12]; // Used to fill with 0 until 512th octer
};


#define TARHEADER static_cast<PosixTarHeader *>(header)
#define RECORD_SIZE sizeof(PosixTarHeader);

class AbstractTar {
public:
    AbstractTar(std::string tarFile);
    virtual ~AbstractTar();
    
    std::string getTarFileName();
    virtual void open() = 0;
protected:
    std::string m_tarFileName;

};

#endif	/* ABSTRACTTAR_H */

