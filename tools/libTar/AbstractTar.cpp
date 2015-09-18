/* 
 * File:   AbstractTar.cpp
 * Author: Dante
 * 
 * Created on 23 avril 2013, 23:14
 */

#include "AbstractTar.h"

AbstractTar::AbstractTar(std::string tarFile) : m_tarFileName(tarFile) {
}

AbstractTar::~AbstractTar() {
}

std::string AbstractTar::getTarFileName()
{
    return m_tarFileName;
}
