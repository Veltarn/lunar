#ifndef EMULATOREXCEPTION_H
#define EMULATOREXCEPTION_H

#include <QString>
#include <exception>

class EmulatorException : std::exception
{
public:
    EmulatorException(const char *msg);
    EmulatorException(const std::string msg);
    EmulatorException(const QString msg);

    virtual ~EmulatorException() throw();

    virtual std::string message() const throw();
    virtual const char* what() const throw();

protected:
    std::string m_msg;
};

#endif // EMULATOREXCEPTION_H
