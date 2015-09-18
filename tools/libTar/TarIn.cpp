/* 
 * File:   TarIn.cpp
 * Author: Dante
 * 
 * Created on 23 avril 2013, 23:39
 */

#include "TarIn.h"

using namespace std;

TarIn::TarIn(string tarFile) : AbstractTar(tarFile)
{
    this->open();
}


TarIn::~TarIn() {
}

void TarIn::open()
{
    m_tarFile.open(m_tarFileName.c_str(), ios::in | ios::binary);
}


TarIn::ErrorType TarIn::extract(std::string& filename, std::string& destination)
{
    return this->extract(filename.c_str(), destination.c_str());
}

TarIn::ErrorType TarIn::extract(const char* filename, const char* destination)
{    
    if(!m_tarFile.is_open())
    {
        return FILE_NOT_OPEN;
    }
    
    m_tarFile.seekg(0, ios::beg);
    
    PosixTarHeader header;
    //Searching for the file in the archive
    //Initializing an invalid position
    int pos = -1;
    
    pos = this->search(filename, &header);
    
    if(pos > -1)
    {
        const char *end;
        long long unsigned int filesize = StringOps::octalStringToInt64(header.size, &end);
        cout << "Extracting " << filesize << " octets of " << filename << endl;
        //Reading the whole file
        char *buffer = new char[filesize];
        cout << pos << " " << filesize << endl;
        m_tarFile.read(buffer, filesize);
        
        //Creating folders if necessary
        FileInfo fi(destination);
        vector<string> foldersList = fi.foldersList();
        
        if(foldersList.size() > 0)
        {
            string accu = "";
            for(unsigned int i = 0; i < foldersList.size(); i++)
            {
                accu += foldersList[i];
                mkdir(accu.c_str());
            }
        }
        std::string destinationFile = destination;
        
        //Verifying if the file contains folders
        FileInfo fileToExtract(header.name);
        vector<string> folderListExtract = fileToExtract.foldersList();
        
        if(folderListExtract.size() > 0)
        {
            string accu = fi.pathName();
            for(unsigned int i = 0; i < folderListExtract.size(); i++)
            {
                accu += folderListExtract[i];
                mkdir(accu.c_str());                
            }
        }
        
        if(fi.fileName() == "")
        {
            destinationFile += filename;
        }
        
        ofstream outFile;
        outFile.open(destinationFile.c_str(), ios::out | ios::binary | ios::trunc);
        outFile.write(buffer, filesize);
        outFile.close();
        delete buffer;
        return NO_ERRORS;
    }
    else
    {
        return END_OF_FILE;
    }
}

void TarIn::extractAll(std::string& destination)
{
    this->extractAll(destination.c_str());
}

void TarIn::extractAll(const char* destination)
{
    m_tarFile.seekg(0, ios::beg);
    FileInfo destinationInfo(destination);
    string destinationPath = destination;
    
    if(destinationInfo.fileName() != "")
    {
        destinationPath = destinationInfo.pathName();
    }
    
    PosixTarHeader header;
    bool endOfRecord = false;
    
    while(!m_tarFile.eof() && !endOfRecord)
    {
        memset(&header, 0, sizeof(PosixTarHeader));
        //Reading header
        m_tarFile.read((char*)&header, sizeof(PosixTarHeader));
        cout << header.name << endl;
        if(this->isLastRecord((char *)&header))
        {
            endOfRecord = true;
            continue;
        }
        
        string name = "";
        if(header.name != 0)
        {
            name = header.name;
        }
        TarIn::ErrorType error = this->extract(name, destinationPath);
        if(error != NO_ERRORS && error != END_OF_FILE)
        {
            cerr << "Error during the extraction of \"" << name << "\"" << endl;
        }
        else
        {
            cout << "Extracted \"" << name << "\" in \"" << destinationPath << "\"" << endl << endl;
        }
        int pos = m_tarFile.tellg();
        this->endRecord(pos);
    }
}

unsigned int TarIn::search(const char* filename, PosixTarHeader* header)
{
    PosixTarHeader tmpHeader;    
    m_tarFile.seekg(0, ios::beg);
    if(!m_tarFile.is_open())
    {
        header = NULL;
        return -1;
    }
    
    m_tarFile.seekg(0, ios::beg);
    //Filling tmpHeader with zeros
    memset(&tmpHeader, 0, sizeof(PosixTarHeader));
    
    bool endOfRecord = false;
    //Parsing file
    while(!m_tarFile.eof() && !endOfRecord)
    {
        //Reading header
        m_tarFile.read((char *)&tmpHeader, sizeof(PosixTarHeader));
        if(this->isLastRecord((char *)&tmpHeader))
        {
            endOfRecord = true;
        }
        
        //If this the file we searched for
        if(strcmp(tmpHeader.name, filename) == 0)
        {
            *header = tmpHeader;
            return m_tarFile.tellg();
        }
        
        const char *end;
        long long unsigned int filesize = StringOps::octalStringToInt64(tmpHeader.size, &end);
        
        //If nothing has been found, we move the cursor after the file's datas
        m_tarFile.seekg(filesize, ios::cur);
        
        //Reaching multiple of 512
        size_t pos = m_tarFile.tellg();
        char c;
        while((pos % sizeof(PosixTarHeader)) != 0)
        {
            m_tarFile.get(c);
            pos++;
        }
    }
    
    header = NULL;
    return -1;
}

unsigned int TarIn::search(std::string filename, PosixTarHeader* header)
{
    return this->search(filename.c_str(), header);
}

/**
 * If the header only contains zeros, this is the end of the file
 * @param buffer
 * @return 
 */
bool TarIn::isLastRecord(const char *buffer)
{
    for(int i = 0; i < 512; i++)
    {
        if(buffer[i] != 0)
            return false;
    }
    return true;
}

void TarIn::endRecord(size_t len)
{
    char c;
    
    while(len % sizeof(PosixTarHeader) != 0)
    {
        m_tarFile.get(c);
        ++len;
    }
}

TarFileInfo TarIn::getTarFileInfo()
{
    TarFileInfo fileinfo;
    fileinfo.entries.resize(0);
    //unsigned int nbFiles = 0;
    //Calculating filesize
    m_tarFile.seekg(0, ios::end);
    fileinfo.filesize = m_tarFile.tellg();
    //Rewinding
    m_tarFile.seekg(0, ios::beg);
    
    bool endOfRecord = false;
    while(!endOfRecord)
    {
        PosixTarHeader header;
        memset(&header, 0, sizeof(PosixTarHeader));
        
        const char *end;
        m_tarFile.read((char*)&header, sizeof(PosixTarHeader));
        
        if(!this->isLastRecord((char *)&header))
        {
            fileinfo.entries.push_back(header);
            long long unsigned int filesize = StringOps::octalStringToInt64(header.size, &end);

            //Jumping file datas
            m_tarFile.seekg(filesize, ios::cur);
            //Reaching multiple of 512
            size_t pos = m_tarFile.tellg();

            this->endRecord(pos);
            
        }
        else
        {
            endOfRecord = true;
        }
    }
    
    fileinfo.nbEntries = fileinfo.entries.size();
    
    return fileinfo;
}
