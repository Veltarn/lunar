#include "emulatorexception.h"


EmulatorException::EmulatorException(const char *msg) : m_msg(msg)
{

}

EmulatorException::EmulatorException(const std::string msg) : m_msg(msg)
{

}

EmulatorException::EmulatorException(const QString msg) : m_msg(msg.toStdString())
{

}

EmulatorException::~EmulatorException() throw() {

}

const char* EmulatorException::what() const throw() {
    return m_msg.c_str();
}

std::string EmulatorException::message() const throw() {
    return m_msg;
}
