#ifndef PLATFORMSEXCEPTION_H
#define PLATFORMSEXCEPTION_H

#include <QString>
#include <exception>

class PlatformsException : public std::exception
{
public:
    PlatformsException(const char *msg);
    PlatformsException(const std::string msg);
    PlatformsException(const QString msg);
    virtual ~PlatformsException() throw();

    virtual const std::string message() const throw();
    virtual const char* what() const throw();

protected:
    std::string m_msg;
};

#endif // PLATFORMSEXCEPTION_H
