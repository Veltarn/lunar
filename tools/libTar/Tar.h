/* 
 * File:   Tar.h
 * Author: Dante
 *
 * Created on 21 avril 2013, 14:21
 */

#ifndef TAR_H
#define	TAR_H

#include "TarIn.h"
#include "TarOut.h"

/*#define WINDOWS
#define BLOCK 512

#include <iostream>
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
#include "StringOps.h"

#define TARHEADER static_cast<PosixTarHeader *>(header)


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

class Tar {
public:
    Tar(std::string tarFile);
    virtual ~Tar();
    
    void put(const char *filename, const std::string &s);
    void put(const char *filename, const char *content);
    void put(const char *filename, const char *content, std::size_t len);
    void putFile(const char *filename);
    void putFile(const char *filename, const char *nameInArchive);
    void putFile(std::string filename);
    void putFile(std::string filename, std::string nameInArchive);
    
    bool extract(const char *filename, const char *destination);
    bool extract(std::string &filename, std::string &destination);
    void extractAll(const char *destination);
    void extractAll(std::string &destination);
    
    /**
     *  Search for "filename" file
     * 
     * @param filename Name of file we want to search
     * @param *header Pointer of PosixTarHeader, used to return file's header if it has been found; NULL value if no file was found or if an error has occured
     * 
     * @return Position of the file in the tar archive. Returns -1 if no file was found or if an error has occured
     *//*
    unsigned int search(const char *filename, PosixTarHeader *header);
    
    unsigned int search(std::string filename, PosixTarHeader *header);
    
    //Clear file content
    void clearFile();
    
    //Open tar file
    void open();
    void finish();
private:
    void init(void *header);
    void checksum(void *header);
    void size(void *header, unsigned long filesize);
    void filename(void *header, const char *filename);
    void endRecord(std::size_t len);
    
    int octalToDec(int octal);
private:
    bool m_finished;
    std::ofstream m_tarFile;
    
    std::string m_tarFileName;
};
*/
#endif	/* TAR_H */

