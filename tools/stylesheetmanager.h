#ifndef STYLESHEETMANAGER_H
#define STYLESHEETMANAGER_H

#include <QDebug>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDir>

class StyleSheetManager : public QObject
{
    Q_OBJECT
public:
    explicit StyleSheetManager(QObject *parent = 0);
    
    bool load(QString filename);
    QString get();
signals:
    
public slots:

private:
    QDir m_cssPath;
    QFile m_cssFile;
};

#endif // STYLESHEETMANAGER_H
