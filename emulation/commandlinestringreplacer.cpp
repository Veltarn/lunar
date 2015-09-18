#include "commandlinestringreplacer.h"

CommandLineStringReplacer::CommandLineStringReplacer() : m_commandLine(QString()), m_result(QString())
{

}

CommandLineStringReplacer::CommandLineStringReplacer(QString commandLine) : m_commandLine(commandLine), m_result(QString())
{

}

void CommandLineStringReplacer::setCommandLinePattern(QString commandLine)
{
    m_commandLine = commandLine;
}

void CommandLineStringReplacer::replace(QString pattern, QString replacer)
{
    m_result = m_commandLine.replace(pattern, replacer);
}

QString CommandLineStringReplacer::getRomArgument(QString romPath)
{
    //m_romPathResult = QString("%rom_path%").replace()
}

QString CommandLineStringReplacer::result()
{
    return m_result;
}

