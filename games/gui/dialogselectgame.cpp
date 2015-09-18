#include "dialogselectgame.h"

DialogSelectGame::DialogSelectGame(ApiGameList &gameList, ApiGame *selectedGame,  QWidget *parent) :
    QDialog(parent),
    m_list(gameList),
    m_selectedGame(selectedGame),
    m_cancelledInput(false)
{
    setMinimumSize(QSize(450, 355));
    setWindowTitle(tr("Remote database search results"));
    initDialog();
    initList();
    initEvents();
    layouting();
}

void DialogSelectGame::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);
    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
}

void DialogSelectGame::initDialog()
{
    m_infoLabel = new QLabel(this);
    QString msgLabel = "";
    QString conjugaison = "";

    if(m_list.size() == 1)
    {
        conjugaison = tr("game has been found");
    }
    else
    {
        conjugaison = tr("games has been found");
    }

    QString addInfos = "<p><b>" + tr("Select one game among this list to be linked with the game you want to create") + "</b></p>";

    msgLabel = QString::number(m_list.size()) + " " + conjugaison;

    if(m_list.size() > 0)
    {
        msgLabel += addInfos;
    }

    m_infoLabel->setText(msgLabel);

    m_cancelButton = new Button(tr("Cancel"), this);
    m_validateButton = new Button(tr("Validate"), this);
}

void DialogSelectGame::initList()
{
    m_listView = new QListView(this);
    QStandardItemModel *model = new QStandardItemModel(this);

    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for(int i = 0; i < m_list.size(); ++i)
    {
        QString gameName = m_list.at(i).getName();

        model->appendRow(new QStandardItem(gameName));
    }

    m_listView->setModel(model);
}

void DialogSelectGame::initEvents()
{
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onClickCancelButton()));
    connect(m_validateButton, SIGNAL(clicked()), this, SLOT(onClickValidateButton()));
    connect(m_listView, SIGNAL(activated(QModelIndex)), this, SLOT(onClickValidateButton()));
}

void DialogSelectGame::layouting()
{
    m_mainLayout = new QVBoxLayout;
    m_buttonLayout = new QHBoxLayout;

    m_buttonLayout->addWidget(m_cancelButton);
    m_buttonLayout->addWidget(m_validateButton);

    m_mainLayout->addWidget(m_infoLabel);
    m_mainLayout->addWidget(m_listView);
    m_mainLayout->addLayout(m_buttonLayout);


    this->setLayout(m_mainLayout);
}

void DialogSelectGame::onClickCancelButton()
{
    m_cancelledInput = true;
    accept();
}

void DialogSelectGame::onClickValidateButton()
{
    QModelIndex mi = m_listView->currentIndex();

    for(ApiGameList::iterator it = m_list.begin(); it != m_list.end(); ++it)
    {
        if(mi.data().toString() == it->getName())
        {
            *m_selectedGame = *it;
        }
    }

    accept();
}

bool DialogSelectGame::hasCancelledInput()
{
    return m_cancelledInput;
}
