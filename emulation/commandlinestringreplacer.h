#ifndef COMMANDLINESTRINGREPLACER_H
#define COMMANDLINESTRINGREPLACER_H

#include <QRegExp>
#include <QString>

class CommandLineStringReplacer
{
public:
    CommandLineStringReplacer();
    CommandLineStringReplacer(QString commandLine);

    void setCommandLinePattern(QString commandLine);
    void replace(QString pattern, QString replacer);
    QString getRomArgument(QString romPath);
    QString result();
private:
    QString m_commandLine;
    QString m_result, m_exepathResult, m_romPathResult, m_argumentsResults;
};

#endif // COMMANDLINESTRINGREPLACER_H
