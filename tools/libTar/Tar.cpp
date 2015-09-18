///* 
// * File:   Tar.cpp
// * Author: Dante
// * 
// * Created on 21 avril 2013, 14:21
// */
//
//#include "Tar.h"
//
//using namespace std;
//
//Tar::Tar(string tarFile) :
//        m_tarFileName(tarFile),
//        m_finished(false)
//{
//    this->open();
//}
//
//Tar::~Tar() {
//}
//
///**
// * Private methods
// */
//void Tar::init(void* header)
//{
//    memset(header, 0, sizeof(PosixTarHeader));
//    sprintf(TARHEADER->ustarIndicator, "ustar  ");
//    sprintf(TARHEADER->mtime, "%011lo", time(NULL));
//#ifdef WINDOWS
//    sprintf(TARHEADER->uid, "%07", 0);
//    sprintf(TARHEADER->gid, "%07", 0);
//    sprintf(TARHEADER->mode, "%07o", 0664);
//    /*char s[UNLEN + 1];
//    GetUserName(s, UNLEN + 1);
//    snprintf(TARHEADER->uname, 32, "%s", s);*/
//#endif
//    snprintf(TARHEADER->uname, 32, "%s", "user");
//    snprintf(TARHEADER->gname, 32, "%s", "users");   
//}
//
//void Tar::checksum(void* header)
//{
//    unsigned int sum = 0;
//    char *p = (char*) header;
//    char *q = p + sizeof(PosixTarHeader);
//    
//    while(p < TARHEADER->checksum)
//    {
//        sum += *p++ & 0xff;
//    }
//    
//    for(int i = 0; i < 8; i++)
//    {
//        sum += ' ';
//        ++p;
//    }
//    
//    while(p < q)
//    {
//        sum += *p++ & 0xff;
//    }
//    
//    sprintf(TARHEADER->checksum, "%06o", sum);
//}
//
//void Tar::size(void* header, unsigned long filesize)
//{
//    sprintf(TARHEADER->size, "%011llo", (long long unsigned int)filesize);
//}
//
//void Tar::filename(void* header, const char *filename)
//{
//    if(filename == NULL || filename[0] == 0 || strlen(filename) >= 100)
//    {
//        //throw("Invalid filename \"" << filename << "\"");
//        cerr << "Invalid filename \"" << filename << "\"" << endl;
//        return;
//    }
//    snprintf(TARHEADER->name, 100, "%s", filename);
//}
//
//void Tar::endRecord(size_t len)
//{
//    char c = '\0';
//    /*
//     * If the data block doesn't end on a position which is multiple of 512,
//     * we have to fill with NULL caracter
//     */
//    while((len % sizeof(PosixTarHeader)) != 0)
//    {
//        m_tarFile.write(&c, sizeof(char));
//        ++len;
//    }
//}
//
///**
// * End of private methods
// */
//
//void Tar::finish()
//{
//    m_finished = true;
//    
//    //The end of the archive is indicated by two 512 octets blocks filled with binary zeros
//    PosixTarHeader header;
//    memset((void*)&header, 0, sizeof(PosixTarHeader));
//    m_tarFile.write((const char*)&header, sizeof(PosixTarHeader));
//    m_tarFile.write((const char*)&header, sizeof(PosixTarHeader));
//    m_tarFile.flush();
//    
//    m_tarFile.close();
//}
//
//void Tar::put(const char* filename, const std::string& s)
//{
//    this->put(filename, s.c_str(), s.size());
//}
//
//void Tar::put(const char* filename, const char* content)
//{
//    this->put(filename, content, strlen(content));
//}
//
//void Tar::put(const char* filename, const char* content, std::size_t len)
//{
//    PosixTarHeader header;
//    this->init((void*)&header);
//    this->filename((void *)&header, filename);
//#ifdef WINDOWS
//    header.typeFlag[0] = 0;
//#endif
//    this->size((void*)&header, len);
//    this->checksum((void *)&header);
//    
//    //Writing header
//    m_tarFile.write((const char*)&header, sizeof(PosixTarHeader));
//    //Writing content
//    m_tarFile.write(content, len);
//    this->endRecord(len);
//}
//
//void Tar::putFile(const char* filename)
//{
//    this->putFile(filename, filename);
//}
//
//void Tar::putFile(const char* filename, const char* nameInArchive)
//{
//    char buffer[BUFSIZ];
//    ifstream file;
//    file.open(filename, ios::in | ios::binary);
//    
//    if(!file.is_open())
//    {
//        //throw("Cannot open " << filename << " " << strerror(errno));
//        cerr << "Cannot open " << filename << " " << strerror(errno) << endl;
//        return;
//    }
//    
//    //Calculating file size
//    file.seekg(0, ios::end);
//    long int filesize = file.tellg();
//    file.seekg(0, ios::beg);
//    
//    PosixTarHeader header;
//    this->init((void*)&header);
//    this->filename((void*)&header, nameInArchive);
//#ifdef WINDOWS
//    header.typeFlag[0] = 0;
//#endif
//    this->size((void*)&header, filesize);
//    this->checksum((void*)&header);
//    
//    //Writing header
//    m_tarFile.write((const char*)&header, sizeof(PosixTarHeader));
//    
//    while(file.read(buffer, 1))
//    {
//        m_tarFile.write(buffer, 1);
//    }
//    file.close();
//    
//    this->endRecord(filesize);
//}
//
//void Tar::putFile(std::string filename)
//{
//    this->putFile(filename.c_str(), filename.c_str());
//}
//
//void Tar::putFile(std::string filename, std::string nameInArchive)
//{
//    this->putFile(filename.c_str(), nameInArchive.c_str());
//}
//
//void Tar::open()
//{
//    m_tarFile.open(m_tarFileName.c_str(), ofstream::out | ofstream::binary | ios::app);
//}
//
//bool Tar::extract(std::string& filename, std::string& destination)
//{
//    this->extract(filename.c_str(), destination.c_str());
//}
//
//bool Tar::extract(const char* filename, const char* destination)
//{
//    ifstream tarFile;
//    
//    tarFile.open(m_tarFileName.c_str(), ios::in | ios::binary);
//    
//    if(!tarFile.is_open())
//    {
//        return false;
//    }
//    
//    tarFile.seekg(0, ios::beg);
//    
//    PosixTarHeader header;
//    //Searching for the file in the archive
//    //Initializing an invalid position
//    unsigned int pos = -1;
//    
//    pos = this->search(filename, &header);
//    if(pos > -1)
//    {
//        const char *end;
//        long long unsigned int filesize = StringOps::octalStringToInt64(header.size, &end);
//        
//        //Reading the whole file
//        char *binaryFile = NULL;
//        tarFile.read((char *)binaryFile, filesize);
//        
//        ofstream outFile;
//        outFile.open(destination, ios::out | ios::binary);
//        
//        if(!outFile.is_open())
//        {
//            return false;
//        }
//        
//        outFile.write(binaryFile, filesize);
//        outFile.close();
//        return true;
//    }
//    else
//    {
//        cout << "1" << endl;
//        return false;
//    }
//}
//
//unsigned int Tar::search(const char* filename, PosixTarHeader* header)
//{
//    ifstream tarFile;
//    PosixTarHeader tmpHeader;
//    
//    tarFile.open(m_tarFileName.c_str(), ios::in | ios::binary);
//    
//    if(!tarFile.is_open())
//    {
//        header = NULL;
//        return -1;
//    }
//    
//    //Filling tmpHeader with zeros
//    memset(&tmpHeader, 0, sizeof(PosixTarHeader));
//    int i = 0;
//    //Parsing file
//    while(!tarFile.eof())
//    {
//        //Reading header
//        tarFile.read((char *)&tmpHeader, sizeof(PosixTarHeader));
//        
//        //If this the file we searched for
//        if(strcmp(tmpHeader.name, filename))
//        {
//            *header = tmpHeader;
//            return tarFile.tellg();
//        }
//        //cout << tmpHeader.name << endl;
//        const char *end;
//        long long unsigned int filesize = StringOps::octalStringToInt64(tmpHeader.size, &end);
//        
//        //If nothing has been found, we move the cursor after the file's datas
//        tarFile.seekg(filesize, ios::cur);
//        
//        //Reaching multiple of 512
//        size_t pos = tarFile.tellg();
//        while((pos % sizeof(PosixTarHeader)) != 0)
//        {
//            tarFile.seekg(1, ios::cur);
//            pos++;
//        }
//        cout << i++ << endl;
//    }
//    
//    header = NULL;
//    return -1;
//}
//
//unsigned int Tar::search(std::string filename, PosixTarHeader* header)
//{
//    return this->search(filename.c_str(), header);
//}
//
//int Tar::octalToDec(int octal)
//{
//    cout << octal << endl;
//    int array[20];
//    int i = 0;
//    int decNum = 0;
//    
//    //Saving each individual digit into array
//    for(i = 0; octal > 0; i++)
//    {
//        array[i] = octal % 10;
//        octal = octal / 10;
//    }
//    
//    for(int power = 0, j = 0; j < i; j++, power++)
//    {
//        decNum = decNum + array[j] * pow(8.0, power);
//    }
//    cout << decNum << endl;
//    return decNum;
//}