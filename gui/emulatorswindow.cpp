#include "emulatorswindow.h"

EmulatorsWindow::EmulatorsWindow(QWidget *parent) : QDialog(parent), m_selectedItem(NULL)
{
    this->initGui();
    this->buildGui();

    this->resize(640, 310);
    this->setWindowTitle(tr("Emulators"));
    this->loadEmulators();
    this->initEvents();
}

void EmulatorsWindow::onAddEmulatorClicked()
{
    AddEmulatorWindow aew(this);
    aew.exec();
    this->loadEmulators();
}

void EmulatorsWindow::emulatorSelected(QModelIndex modelIndex)
{
    if(modelIndex.isValid()) {
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(m_emulatorsView->model());
        QModelIndex firstColumn = modelIndex.sibling(modelIndex.row(), 0);

        QStandardItem *item = model->itemFromIndex(firstColumn);
        m_selectedItem = item;
        m_removeEmulatorAction->setEnabled(true);
    } else {
        m_selectedItem = NULL;
        m_removeEmulatorAction->setEnabled(false);
    }
}

void EmulatorsWindow::removeEmulator()
{
    if(m_selectedItem) {
        int response = QMessageBox::question(this, tr("Confirmation"), tr("Do you want to remove the selected emulator?"), QMessageBox::Yes|QMessageBox::No);

        if(response == QMessageBox::Yes) {
            int id = m_selectedItem->data().toInt();
            try {
                Emulators::removeEmulator(id);
            } catch(EmulatorException e) {
                QMessageBox::critical(this, tr("Error"), QString::fromStdString(e.message()));
                return;
            }
            this->removeEmulatorJunction(id);
            this->loadEmulators();
        }
    }
}

void EmulatorsWindow::startEmulator()
{
    QItemSelectionModel *selection = m_emulatorsView->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();
    qDebug() << indexes.size() << endl;
    if(indexes.size() == 4) {
        QModelIndex index = indexes[1];

        QString path = index.data().toString();
        ProcessManager *pm = ProcessManager::getInstance();
        pm->startProgram(path);
    }
}

void EmulatorsWindow::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

     QPainter painter(this);

     QColor bgColor(255, 255, 255);
     QBrush brush(bgColor);
     painter.setPen(Qt::NoPen);
     painter.setBrush(brush);

     painter.drawRect(0, 0, this->width(), this->height());

}

void EmulatorsWindow::initGui()
{
    m_mainLayout = new QHBoxLayout(this);
    m_toolbar = new QToolBar("Emulators Toolbar", this);
    m_addEmulatorAction = new QAction(QIcon(QPixmap("resources/imgs/add.png")), tr("Add Emulator"), this);
    m_removeEmulatorAction = new QAction(QIcon(QPixmap("resources/imgs/remove.png")), tr("Remove Emulator"), this);
    m_startEmulatorAction = new QAction(QIcon(QPixmap("resources/imgs/start.png")), tr("Start Emulator"), this);
    m_removeEmulatorAction->setDisabled(true);
    m_emulatorsView = new QTreeView(this);
    m_emulatorsView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void EmulatorsWindow::initEvents()
{
    connect(m_removeEmulatorAction, SIGNAL(triggered()), this, SLOT(removeEmulator()));
    connect(m_emulatorsView, SIGNAL(clicked(QModelIndex)), this, SLOT(emulatorSelected(QModelIndex)));
    connect(m_addEmulatorAction, SIGNAL(triggered()), this, SLOT(onAddEmulatorClicked()));
    connect(m_startEmulatorAction, SIGNAL(triggered()), this, SLOT(startEmulator()));
}

void EmulatorsWindow::buildGui()
{
    m_toolbar->addAction(m_addEmulatorAction);
    m_toolbar->addAction(m_removeEmulatorAction);
    m_toolbar->addAction(m_startEmulatorAction);

    m_mainLayout->addWidget(m_emulatorsView);

    setLayout(m_mainLayout);
    this->layout()->setMenuBar(m_toolbar);
}

void EmulatorsWindow::loadEmulators()
{
    QStringList modelHeader;
    modelHeader << tr("Emulator Name") << tr("Exe path") << tr("Command Line Pattern") << tr("Creation Date")/* << tr("Platform")*/;
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHorizontalHeaderLabels(modelHeader);

    Database database = Database();
    database.open();
    QSqlQuery query(database.db());
    QString sql = "SELECT emulator.id AS emulator_id, emulator.name, emulator.creation_date, emulator.exe_path, emulator.command_line_pattern, emulator.emulated_platform FROM emulator";

    if(!query.exec(sql)) {
        QString errorMessage(QString(tr("Cannot load emulators list.") + "\n" + tr("Reason") + ": " + query.lastError().text()));
        QMessageBox::critical(this, tr("Error"), errorMessage);
    } else {
        while(query.next()) {
            QSqlRecord record = query.record();
            QList<QStandardItem*> row;

            int emulatorId = record.value("emulator_id").toInt();
            QString emulatorName = record.value("name").toString();
            //QDateTime creationDate = QDateTime::fromString(record.value("creation_date").toD)
            QDateTime creationDate = record.value("creation_date").toDateTime();
            QString exePath = record.value("exe_path").toString();
            QString commandLinePattern = record.value("command_line_pattern").toString();
            QList<Platform> emulatedPlatforms;
            try {
                emulatedPlatforms = Platforms::getPlatformsByEmulatorId(emulatorId);
            } catch(PlatformsException &e) {
                QMessageBox::critical(this, tr("Error"), QString::fromStdString(e.message()));
                exit(EXIT_FAILURE); //A little bit rude
            }

            QList<QStandardItem *> platforms = this->createPlatformsSubtree(emulatedPlatforms);

            QStandardItem *itemEmulatorName = new QStandardItem(emulatorName);
            itemEmulatorName->setData(emulatorId);
            itemEmulatorName->setEditable(false);

            QStandardItem *itemCreationDate = new QStandardItem(creationDate.toString(STANDARD_FR_DATETIME));
            itemCreationDate->setData(creationDate);
            itemCreationDate->setEditable(false);

            QStandardItem *itemExePath = new QStandardItem(exePath);
            itemExePath->setData(exePath);
            itemExePath->setEditable(false);

            QStandardItem *itemCommandLinePattarn = new QStandardItem(commandLinePattern);
            itemCommandLinePattarn->setData(commandLinePattern);
            itemCommandLinePattarn->setEditable(false);

            /*QStandardItem *itemEmulatedPlatform = new QStandardItem(emulatedPlatform);
            itemEmulatedPlatform->setData(record.value("platform_id").toInt());
            itemEmulatedPlatform->setEditable(false);*/

            //Adding platforms as a subtree
            foreach(QStandardItem *row, platforms) {
                itemEmulatorName->appendRow(row);
            }

            row << itemEmulatorName << /*itemEmulatedPlatform <<*/ itemExePath << itemCommandLinePattarn << itemCreationDate ;
            model->appendRow(row);
        }
        m_emulatorsView->setModel(model);
    }
    database.close();
}

QList<QStandardItem *> EmulatorsWindow::createPlatformsSubtree(QList<Platform> &platforms)
{
    QList<QStandardItem *> items;

    foreach(Platform platform, platforms) {

        QStandardItem *platformName = new QStandardItem(platform.platform_name);
        platformName->setData(platform.platform_id);
        platformName->setEditable(false);

        items << platformName;
    }
    return items;
}

void EmulatorsWindow::removeEmulatorJunction(int emulatorId)
{
    Database database;
    database.open();

    QSqlQuery query(database.db());
    QString sql("DELETE FROM emulators_platforms_junction WHERE emulator_id = :emulatorId");

    if(!query.prepare(sql)) {
        QString message(tr("An error occured while operating on the emulator ") + "\n" + query.lastError().text());
        database.close();
        QMessageBox::critical(this, tr("Error"), message);
    } else {
        query.bindValue(":emulatorId", emulatorId);
        query.exec();
    }
}
