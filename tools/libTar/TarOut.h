/* 
 * File:   TarOut.h
 * Author: Dante
 *
 * Created on 23 avril 2013, 23:06
 */

#ifndef TAROUT_H
#define	TAROUT_H

#include "AbstractTar.h"

class TarOut : public AbstractTar{
public:
    TarOut(std::string tarFile, const char *openmode);
    virtual ~TarOut();
        
    void put(const char *filename, const std::string &s);
    void put(const char *filename, const char *content);
    void put(const char *filename, const char *content, std::size_t len);
    void putFile(const char *filename);
    void putFile(const char *filename, const char *nameInArchive);
    void putFile(std::string filename);
    void putFile(std::string filename, std::string nameInArchive);
    
    virtual void open();
    virtual void finish();
private: 
    void checkOpenState();
    void init(void *header);
    void checksum(void *header);
    void size(void *header, unsigned long filesize);
    void filename(void *header, const char *filename);
    void endRecord(std::size_t len);
    
private:        
    bool m_finished;
    std::ofstream m_tarFile;
    const char *m_openmode;
};

#endif	/* TAROUT_H */

