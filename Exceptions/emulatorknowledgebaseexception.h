#ifndef EMULATORKNOWLEDGEBASEEXCEPTION_H
#define EMULATORKNOWLEDGEBASEEXCEPTION_H

#include <QString>
#include <exception>

class EmulatorKnowledgeBaseException : std::exception
{
public:
    EmulatorKnowledgeBaseException(const char *msg);
    EmulatorKnowledgeBaseException(const std::string msg);
    EmulatorKnowledgeBaseException(const QString msg);

    virtual ~EmulatorKnowledgeBaseException() throw();

    virtual std::string message() const throw();
    virtual const char* what() const throw();

protected:
    std::string m_msg;
};

#endif // EMULATORKNOWLEDGEBASEEXCEPTION_H
