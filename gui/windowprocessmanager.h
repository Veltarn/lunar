#ifndef WINDOWPROCESSMANAGER_H
#define WINDOWPROCESSMANAGER_H

#include <iostream>
#include <QDialog>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <QCloseEvent>
#include "../tools/processmanager.h"
#include "../tools/process.h"
#include "button.h"

class WindowProcessManager : public QDialog
{
    Q_OBJECT
    
public:
    explicit WindowProcessManager(QWidget *parent = 0);
    ~WindowProcessManager();
    
public slots:
    void onTimeout();

protected:
    void paintEvent(QPaintEvent *pe);

private:
    void initData();
    void updateData();
    void msToRegularTime(int *h, int *m, int *s, int ms);
private:

    Button *m_actionTerminateProcess;
    Button *m_actionKillProcess;

    QHBoxLayout *m_buttonLayout;
    QVBoxLayout *m_mainLayout;

    QTableView *m_processesList;

    ProcessManager *m_processManager;

    QTimer m_tick;
};

#endif // WINDOWPROCESSMANAGER_H
