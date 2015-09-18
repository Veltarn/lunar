#include "emulatorknowledgebaseexception.h"

EmulatorKnowledgeBaseException::EmulatorKnowledgeBaseException(const char *msg) : m_msg(msg)
{

}

EmulatorKnowledgeBaseException::EmulatorKnowledgeBaseException(const std::string msg) : m_msg(msg)
{

}

EmulatorKnowledgeBaseException::EmulatorKnowledgeBaseException(const QString msg) : m_msg(msg.toStdString())
{

}

EmulatorKnowledgeBaseException::~EmulatorKnowledgeBaseException() throw() {

}

const char* EmulatorKnowledgeBaseException::what() const throw() {
    return m_msg.c_str();
}

std::string EmulatorKnowledgeBaseException::message() const throw() {
    return m_msg;
}
