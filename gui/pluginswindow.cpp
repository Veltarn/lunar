#include "pluginswindow.h"

PluginsWindow::PluginsWindow(QWidget *parent) : QDialog(parent), m_selectedItem(NULL)
{
    initGui();
    buildGui();
    getPluginsData();
    initEvents();

    this->resize(640, 360);
}

void PluginsWindow::onEnable()
{
    QPluginLoader *pl = PluginManager::getInstance()->getPluginByName(m_selectedItem->text());
    QString name = pl->metaData().value("MetaData").toObject().value("name").toString();

    PluginManager::getInstance()->setPluginEnabled(name, true);
    getPluginsData();
}

void PluginsWindow::onDisable()
{
    QPluginLoader *pl = PluginManager::getInstance()->getPluginByName(m_selectedItem->text());
    QString name = pl->metaData().value("MetaData").toObject().value("name").toString();

    PluginManager::getInstance()->setPluginEnabled(name, false);
    getPluginsData();
}

void PluginsWindow::closeWindow()
{
    this->close();
}

void PluginsWindow::onValidate()
{
    this->close();
}

void PluginsWindow::onSelectPlugin(QModelIndex index)
{
    if(index.isValid()) {
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(m_pluginView->model());
        QModelIndex firstColumn = index.sibling(index.row(), 1);

        QStandardItem *item = model->itemFromIndex(firstColumn);
        m_enablePluginAction->setEnabled(true);
        m_disablePluginAction->setEnabled(true);
        m_selectedItem = item;
        qDebug() << item->text() << endl;
    } else {
        m_selectedItem = NULL;
        m_enablePluginAction->setEnabled(false);
        m_disablePluginAction->setEnabled(false);
    }
}

void PluginsWindow::paintEvent(QPaintEvent *)
{
   QPainter painter(this);

   QColor bgColor(255, 255, 255);
   QBrush brush(bgColor);
   painter.setPen(Qt::NoPen);
   painter.setBrush(brush);

   painter.drawRect(0, 0, this->width(), this->height());
}

void PluginsWindow::getPluginsData()
{
    m_model->clear();
    QStringList header;
    header << tr("Enabled") << tr("Plugin Name") << tr("Filename") << tr("Author") << tr("Version") << tr("Description");
    m_model->setHorizontalHeaderLabels(header);
    populateModel();
    m_pluginView->setModel(m_model);
    m_enablePluginAction->setEnabled(false);
    m_disablePluginAction->setEnabled(false);
}

void PluginsWindow::initGui()
{
    m_toolbar = new QToolBar("Toolbar", this);

    m_enablePluginAction = new QAction(QIcon(QPixmap("resources/imgs/enablePlugin.png")), tr("Enable Plugin"), this);
    m_disablePluginAction = new QAction(QIcon(QPixmap("resources/imgs/disablePlugin.png")), tr("Disable Plugin"), this);

    m_enablePluginAction->setEnabled(false);
    m_disablePluginAction->setEnabled(false);

    m_toolbar->addAction(m_enablePluginAction);
    m_toolbar->addAction(m_disablePluginAction);

    m_validateButton = new Button(tr("Validate"), this);
    m_closeButton = new Button(tr("Close"), this);

    m_pluginView = new QTreeView(this);
    m_model = new QStandardItemModel(this);

    m_mainLayout = new QVBoxLayout();
    m_buttonsLayout = new QHBoxLayout();
}

void PluginsWindow::buildGui()
{
    m_buttonsLayout->addWidget(m_validateButton);
    m_buttonsLayout->addWidget(m_closeButton);

    m_mainLayout->addWidget(m_toolbar);
    m_mainLayout->addWidget(m_pluginView);
    m_mainLayout->addLayout(m_buttonsLayout);

    this->setLayout(m_mainLayout);
}

void PluginsWindow::populateModel()
{
    PluginManager *pm = PluginManager::getInstance();

    foreach(QPluginLoader *plugin, pm->pluginsList()) {
        QJsonObject metadata = plugin->metaData().value("MetaData").toObject();
        QString name = metadata.value("name").toString();
        QString description = metadata.value("description").toString();
        QString author = metadata.value("author").toString();
        QString version = metadata.value("version").toString();
        QString filepath = plugin->fileName();
        bool enabled = PluginManager::isPluginEnabled(name);

        QString strEnabled = enabled ? "*" : "";

        QStandardItem *enabledItem = new QStandardItem(strEnabled);
        enabledItem->setEditable(false);

        QStandardItem *nameItem = new QStandardItem(name);
        nameItem->setEditable(false);

        QStandardItem *descriptionItem = new QStandardItem(description);
        descriptionItem->setEditable(false);

        QStandardItem *authorItem = new QStandardItem(author);
        authorItem->setEditable(false);

        QStandardItem *versionItem = new QStandardItem(version);
        versionItem->setEditable(false);

        QStandardItem *filepathItem = new QStandardItem(filepath);
        filepathItem->setEditable(false);

        QList<QStandardItem*> list;
        list << enabledItem << nameItem << filepathItem << authorItem << versionItem << descriptionItem;

        m_model->appendRow(list);
    }
}

void PluginsWindow::initEvents()
{
    connect(m_pluginView, SIGNAL(clicked(QModelIndex)), this, SLOT(onSelectPlugin(QModelIndex)));
    connect(m_enablePluginAction, SIGNAL(triggered()), this, SLOT(onEnable()));
    connect(m_disablePluginAction, SIGNAL(triggered()), this, SLOT(onDisable()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(m_validateButton, SIGNAL(clicked()), this, SLOT(onValidate()));
}

