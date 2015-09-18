/* 
 * File:   FileInfo.cpp
 * Author: Dante
 * 
 * Created on 24 avril 2013, 14:05
 */

#include "FileInfo.h"

using namespace std;

FileInfo::FileInfo(string filename) : m_filename(filename) {
}

FileInfo::FileInfo(const char *filename) : m_filename(filename) {
}

FileInfo::~FileInfo() {
}

string FileInfo::pathName() {
    unsigned int found = m_filename.find_last_of("/\\");

    //If found position == filename position
    if (found == m_filename.size() - 1) {
        return m_filename;
    } else {
        return m_filename.substr(0, found);
    }
}

string FileInfo::fileName() {
    unsigned int found = m_filename.find_last_of("/\\");

    //If found position == filename position
    if (found == m_filename.size() - 1) {
        return "";
    } else {
        return m_filename.substr(found + 1);
    }
}

string FileInfo::extension() {
    unsigned int found = m_filename.find_last_of(".");

    if (found != string::npos) {
        return m_filename.substr(found + 1);
    }

    return string("");
}

vector<string> FileInfo::foldersList() {
    vector<string> foldersList;
    
    unsigned int i = 0;
    unsigned int pos = string::npos;
    do {
        pos = m_filename.find_first_of("/\\", i);        
        if (pos != string::npos)
        {
            unsigned int offset = pos - i;
            string tmp = m_filename.substr(i, offset + 1);
            foldersList.push_back(tmp);
            i = pos + 1;
        }
    } while (pos != string::npos);
    
    return foldersList;
}
