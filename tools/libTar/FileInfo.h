/* 
 * File:   FileInfo.h
 * Author: Dante
 *
 * Created on 24 avril 2013, 14:05
 */

#ifndef FILEINFO_H
#define	FILEINFO_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

class FileInfo {
public:
    FileInfo(std::string filename);
    FileInfo(const char *filename);
    virtual ~FileInfo();
    
    std::string pathName();
    std::string fileName();
    /**
     * 
     * @return List of folders of a path
     */
    std::vector<std::string> foldersList();
    std::string extension();
private:
    std::string m_filename;

};

#endif	/* FILEINFO_H */

