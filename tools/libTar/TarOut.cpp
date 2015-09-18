/* 
 * File:   TarOut.cpp
 * Author: Dante
 * 
 * Created on 23 avril 2013, 23:06
 */

#include "TarOut.h"

using namespace std;

TarOut::TarOut(string tarFile, const char *openmode) : AbstractTar(tarFile), m_finished(false), m_openmode(openmode)
{
}

TarOut::~TarOut() {
    if(!m_finished)
    {
        cerr << "[WARNING]Tar file non finished" << endl;
    }
}

void TarOut::open()
{
    if(strcmp(m_openmode, "app"))
    {
        m_tarFile.open(m_tarFileName.c_str(), ios::out | ios::binary | ios::app);
    }
    else
    {
        m_tarFile.open(m_tarFileName.c_str(), ios::out | ios::binary);
    }
}

/**
 * Private methods
 */
void TarOut::init(void* header)
{
    memset(header, 0, sizeof(PosixTarHeader));
    sprintf(TARHEADER->ustarIndicator, "ustar  ");
    sprintf(TARHEADER->mtime, "%011lo", time(NULL));
#ifdef WINDOWS
    sprintf(TARHEADER->uid, "%07", 0);
    sprintf(TARHEADER->gid, "%07", 0);
    sprintf(TARHEADER->mode, "%07o", 0664);
    /*char s[UNLEN + 1];
    GetUserName(s, UNLEN + 1);
    snprintf(TARHEADER->uname, 32, "%s", s);*/
#endif
    snprintf(TARHEADER->uname, 32, "%s", "user");
    snprintf(TARHEADER->gname, 32, "%s", "users");   
}

void TarOut::checksum(void* header)
{
    unsigned int sum = 0;
    char *p = (char*) header;
    char *q = p + sizeof(PosixTarHeader);
    
    while(p < TARHEADER->checksum)
    {
        sum += *p++ & 0xff;
    }
    
    for(int i = 0; i < 8; i++)
    {
        sum += ' ';
        ++p;
    }
    
    while(p < q)
    {
        sum += *p++ & 0xff;
    }
    
    sprintf(TARHEADER->checksum, "%06o", sum);
}

void TarOut::size(void* header, unsigned long filesize)
{
    sprintf(TARHEADER->size, "%011llo", (long long unsigned int)filesize);
}

void TarOut::filename(void* header, const char *filename)
{
    if(filename == NULL || filename[0] == 0 || strlen(filename) >= 100)
    {
        //throw("Invalid filename \"" << filename << "\"");
        cerr << "Invalid filename \"" << filename << "\"" << endl;
        return;
    }
    snprintf(TARHEADER->name, 100, "%s", filename);
}

void TarOut::endRecord(size_t len)
{
    char c = '\0';
    /*
     * If the data block doesn't end on a position which is multiple of 512,
     * we have to fill with NULL caracter
     */
    while((len % sizeof(PosixTarHeader)) != 0)
    {
        m_tarFile.write(&c, sizeof(char));
        ++len;
    }
}

/**
 * End of private methods
 */

void TarOut::finish()
{
    m_finished = true;
    
    //The end of the archive is indicated by two 512 octets blocks filled with binary zeros
    PosixTarHeader header;
    memset((void*)&header, 0, sizeof(PosixTarHeader));
    m_tarFile.write((const char*)&header, sizeof(PosixTarHeader));
    m_tarFile.write((const char*)&header, sizeof(PosixTarHeader));
    m_tarFile.flush();
    
    m_tarFile.close();
}

void TarOut::put(const char* filename, const std::string& s)
{
    this->put(filename, s.c_str(), s.size());
}

void TarOut::put(const char* filename, const char* content)
{
    this->put(filename, content, strlen(content));
}

void TarOut::put(const char* filename, const char* content, std::size_t len)
{
    checkOpenState();
    PosixTarHeader header;
    this->init((void*)&header);
    this->filename((void *)&header, filename);
#ifdef WINDOWS
    header.typeFlag[0] = 0;
#endif
    this->size((void*)&header, len);
    this->checksum((void *)&header);
    
    //Writing header
    m_tarFile.write((const char*)&header, sizeof(PosixTarHeader));
    //Writing content
    m_tarFile.write(content, len);
    this->endRecord(len);
}

void TarOut::putFile(const char* filename)
{
    this->putFile(filename, filename);
}

void TarOut::putFile(const char* filename, const char* nameInArchive)
{
    checkOpenState();
    char buffer[BUFSIZ];
    ifstream file;
    file.open(filename, ios::in | ios::binary);
    
    if(!file.is_open())
    {
        //throw("Cannot open " << filename << " " << strerror(errno));
        cerr << "Cannot open " << filename << " " << strerror(errno) << endl;
        return;
    }
    
    //Calculating file size
    file.seekg(0, ios::end);
    long int filesize = file.tellg();
    file.seekg(0, ios::beg);
    
    PosixTarHeader header;
    this->init((void*)&header);
    this->filename((void*)&header, nameInArchive);
#ifdef WINDOWS
    header.typeFlag[0] = 0;
#endif
    this->size((void*)&header, filesize);
    this->checksum((void*)&header);
    
    //Writing header
    m_tarFile.write((const char*)&header, sizeof(PosixTarHeader));
    
    while(file.read(buffer, 1))
    {
        m_tarFile.write(buffer, 1);
    }
    file.close();
    
    this->endRecord(filesize);
}

void TarOut::putFile(std::string filename)
{
    this->putFile(filename.c_str(), filename.c_str());
}

void TarOut::putFile(std::string filename, std::string nameInArchive)
{
    this->putFile(filename.c_str(), nameInArchive.c_str());
}

 void TarOut::checkOpenState()
 {
     if(!m_tarFile.is_open())
     {
         this->open();
     }
 }
