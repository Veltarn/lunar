#ifndef PLUGINSEXCEPTION_H
#define PLUGINSEXCEPTION_H

#include <QString>
#include <exception>

class PluginsException : public std::exception
{
public:
    PluginsException(const char *msg);
    PluginsException(const std::string msg);
    PluginsException(const QString msg);
    virtual ~PluginsException() throw();

    virtual const std::string message() const throw();
    virtual const char* what() const throw();

protected:
    std::string m_msg;
};

#endif // PLUGINSEXCEPTION_H
