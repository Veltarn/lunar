#include "pluginsexception.h"

PluginsException::PluginsException(const char *msg) : m_msg(msg)
{

}

PluginsException::PluginsException(const std::string msg) : m_msg(msg)
{

}

PluginsException::PluginsException(const QString msg) : m_msg(msg.toStdString())
{

}

PluginsException::~PluginsException() throw() {

}

const char* PluginsException::what() const throw() {
    return m_msg.c_str();
}

const std::string PluginsException::message() const throw() {
    return m_msg;
}

