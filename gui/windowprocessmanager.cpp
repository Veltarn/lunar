#include "windowprocessmanager.h"
//#include "ui_windowprocessmanager.h"

using namespace std;

WindowProcessManager::WindowProcessManager(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Processes manager"));

    m_mainLayout = new QVBoxLayout;
    m_buttonLayout = new QHBoxLayout;

    m_processManager = ProcessManager::getInstance();
    this->setMinimumSize(450, 340);

    m_actionTerminateProcess = new Button(tr("Terminate"), this);
    m_actionKillProcess = new Button(tr("Kill"), this);

    m_actionTerminateProcess->setIcon(QIcon(QPixmap("resources/imgs/terminate.png")));
    m_actionKillProcess->setIcon(QIcon(QPixmap("resources/imgs/kill.png")));

    m_actionTerminateProcess->setToolTip(tr("Gently stop the game.<br />Might not work if the game is crashed"));
    m_actionKillProcess->setToolTip(tr("Brutally kill the game"));

    m_buttonLayout->addWidget(m_actionTerminateProcess, 1, Qt::AlignLeft);
    m_buttonLayout->addWidget(m_actionKillProcess, 4, Qt::AlignLeft);

    m_processesList = new QTableView(this);
    m_processesList->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*QString stylesheet = "";
    stylesheet = "QTableView { border: 0 }";
    m_processesList->setStyleSheet(stylesheet);*/

    m_mainLayout->addLayout(m_buttonLayout, 1);
    m_mainLayout->addWidget(m_processesList, 4);

    this->initData();

    this->setLayout(m_mainLayout);
}

WindowProcessManager::~WindowProcessManager()
{
}

void WindowProcessManager::initData()
{
    QMap<Process*, QModelIndex> processesList = m_processManager->getList();

    QStandardItemModel *model = new QStandardItemModel(processesList.size(), 4);


    m_processesList->setModel(model);

    this->updateData();

    m_tick.setInterval(2500);
    m_tick.start();

    connect(&m_tick, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void WindowProcessManager::onTimeout()
{
    this->updateData();
}

//Soucis de lising
void WindowProcessManager::updateData()
{
    QMap<Process*, QModelIndex> processesList = m_processManager->getList();
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(m_processesList->model());

    model->clear();

    model->setHeaderData(0, Qt::Horizontal, tr("Game name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Game time"));
    model->setHeaderData(2, Qt::Horizontal, tr("Executable path"));
    model->setHeaderData(3, Qt::Horizontal, tr("Process state"));

    int i = 0;
    for(QMap<Process*, QModelIndex>::iterator it = processesList.begin(); it != processesList.end(); ++it)
    {
        QModelIndex miPath = it->sibling(it->row(), 2);
        QString gameName = it->data().toString();
        QString path = miPath.data().toString();
        QString processState = "";
        int playedTime = it.key()->getPlayedTime();

        int secs = 0;
        int minute = 0;
        int hour = 0;

        msToRegularTime(&hour, &minute, &secs, playedTime);

        QString strTime = QString::number(hour) + ":" + QString::number(minute) + ":" + QString::number(secs);

        switch(it.key()->state())
        {
             case QProcess::NotRunning:
                processState = tr("Stopped");
             break;

             case QProcess::Starting:
                    processState = tr("Starting");
             break;

             case QProcess::Running:
                processState = tr("Running");
             break;
        }

        model->setItem(i, 0, new QStandardItem(gameName));
        model->setItem(i, 1, new QStandardItem(strTime));
        model->setItem(i, 2, new QStandardItem(path));
        model->setItem(i, 3, new QStandardItem(processState));
        i++;
    }
}

void WindowProcessManager::msToRegularTime(int *h, int *m, int *s, int ms)
{
    *s = ms / 1000;

    *m = *s / 60;
    *s %= 60;

    *h = *m / 60;
    *m %= 60;
}

void WindowProcessManager::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);

    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
}
