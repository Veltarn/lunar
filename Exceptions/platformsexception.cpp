#include "platformsexception.h"

PlatformsException::PlatformsException(const char *msg) : m_msg(msg)
{

}

PlatformsException::PlatformsException(const std::string msg) : m_msg(msg)
{

}

PlatformsException::PlatformsException(const QString msg) : m_msg(msg.toStdString())
{

}

PlatformsException::~PlatformsException() throw() {

}

const char* PlatformsException::what() const throw() {
    return m_msg.c_str();
}

const std::string PlatformsException::message() const throw() {
    return m_msg;
}
