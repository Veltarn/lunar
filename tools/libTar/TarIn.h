/* 
 * File:   TarIn.h
 * Author: Dante
 *
 * Created on 23 avril 2013, 23:39
 */

#ifndef TARIN_H
#define	TARIN_H

#include "AbstractTar.h"

struct TarFileInfo
{
    unsigned int nbEntries;
    long long unsigned int filesize;
    std::vector<PosixTarHeader> entries;
};

class TarIn : public AbstractTar
{
public:

    enum ErrorType {
        NO_ERRORS, FILE_NOT_OPEN, END_OF_FILE
    };
public:
    TarIn(std::string tarFile);
    virtual ~TarIn();

    ErrorType extract(const char *filename, const char *destination);
    ErrorType extract(std::string &filename, std::string &destination);
    void extractAll(const char *destination);
    void extractAll(std::string &destination);

    /**
     *  Search for "filename" file
     * 
     * @param filename Name of file we want to search
     * @param *header Pointer of PosixTarHeader, used to return file's header if it has been found; NULL value if no file was found or if an error has occured
     * 
     * @return Position of the file in the tar archive. Returns -1 if no file was found or if an error has occured
     */
    unsigned int search(const char *filename, PosixTarHeader *header);

    unsigned int search(std::string filename, PosixTarHeader *header);
    
    TarFileInfo getTarFileInfo();
    
    virtual void open();

private:
    bool isLastRecord(const char *buffer);
    void endRecord(size_t len);
private:
    std::ifstream m_tarFile;
};

#endif	/* TARIN_H */

