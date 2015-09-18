#include "addemulatorwindow.h"

AddEmulatorWindow::AddEmulatorWindow(QWidget *parent) : QDialog(parent)
{
    this->initGui();
    this->buildGui();

    this->setWindowTitle(tr("New Emulator"));
    this->initEvents();
    this->resize(470, 320);
}

void AddEmulatorWindow::onBrowseEmulator()
{
    QString fileFilter;
#ifdef WIN32
    fileFilter = QString(QObject::tr("Executable files") + " (*.exe)");
#endif
    QString filepath = QFileDialog::getOpenFileName(this, tr("Select the emulator executable file"), QString(), fileFilter);

    if(!filepath.isEmpty()) {
        EmulatorsKnowledgeBase emulatorKnowledgeBase;
        EmulatorKnowledge knowledge = emulatorKnowledgeBase.getKnowledgeFromPath(filepath);

        m_exePathLineEdit->setText(filepath);
        if(knowledge.valid) {
            m_emulatorNameLineEdit->setText(knowledge.emulatorName);
            m_commandLinePatternText->setText(knowledge.commandLinePattern);
            m_emulatedPlatforms->setAllCheckboxes(false);
            if(knowledge.emulatedPlatforms.size() > 0) {
                foreach(Platform platform, knowledge.emulatedPlatforms) {
                    m_emulatedPlatforms->setCheckboxStateByData(platform.platform_id, true);
                }
            }
            /*if(knowledge.emulatedPlatforms > -1) {
                int index = m_emulatedPlatforms->findData(knowledge.emulatedPlatforms);
                if(index > -1) {
                    m_emulatedPlatforms->setCurrentIndex(index);
                }
            }*/
        }
    }
}

void AddEmulatorWindow::closeWindow()
{
    this->close();
}

void AddEmulatorWindow::onValidate()
{
    QString emulatorName = m_emulatorNameLineEdit->text();
    QString exePath = m_exePathLineEdit->text();

    if(!emulatorName.isEmpty() && !exePath.isEmpty()) {
        QString commandLineEditPattern = m_commandLinePatternText->toPlainText();
        QList<QDataCheckBox *> selectedBox = m_emulatedPlatforms->checkedBox();
        QDateTime datetime = QDateTime::currentDateTime();

        QString date = datetime.date().toString("yyyy-MM-dd");
        date += " " + datetime.time().toString("HH:mm:ss");

        Database database;
        database.open();

        QSqlQuery query(database.db());
        QString sql("INSERT INTO emulator('name', 'creation_date', 'exe_path', 'command_line_pattern') VALUES(:name, :date, :exePath, :commandLinePattern)");

        if(!query.prepare(sql)) {
            QString message(tr("An error occured while validating the emulator ") + "\n" + query.lastError().text());
            database.close();
            QMessageBox::critical(this, tr("Error"), message);
        } else {
            query.bindValue(":name", emulatorName);
            query.bindValue(":date", date);
            query.bindValue(":exePath", exePath);
            query.bindValue(":commandLinePattern", commandLineEditPattern);

            query.exec();

            int lastId = query.lastInsertId().toInt();

            //Iterating through selectedBox to insert the junctions between platform table and emulator
            foreach(QDataCheckBox* checkbox, selectedBox) {
                int platformId = checkbox->data().toInt();
                this->createEmulatorPlatformJunction(lastId, platformId);
            }

            QMessageBox::information(this, tr("Success"), tr("The emulator have been saved"));
        }

        database.close();
        this->close();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Cannot save the emulator, the name or the path hadn't been set"));
    }
}

void AddEmulatorWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
}

void AddEmulatorWindow::initGui()
{
    m_mainLayout = new QVBoxLayout;
    m_formLayout = new QFormLayout;

    m_emulatorNameLabel = new QLabel(tr("Emulator Name"), this);
    m_emulatorPathLabel = new QLabel(tr("Emulator Path"), this);
    m_emulatorPlatformsLabel = new QLabel(tr("Emulator Platforms"), this);
    m_commandLinePatternLabel = new QLabel(tr("Command Line Pattern"), this);

    m_validateButton = new Button(tr("Ok"));
    m_cancelButton = new Button(tr("Cancel"));
    m_browseExeButton = new Button(tr("Browse"));

    m_emulatorNameLineEdit = new QLineEdit();
    m_exePathLineEdit = new QLineEdit();
    m_commandLinePatternText = new QTextEdit(this);
    m_emulatedPlatforms = new QMultipleChoices(this);

    this->populateComboBox();
}

void AddEmulatorWindow::buildGui()
{
    QHBoxLayout *exePathLayout = new QHBoxLayout();
    exePathLayout->addWidget(m_exePathLineEdit, 2);
    exePathLayout->addWidget(m_browseExeButton, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_validateButton);
    buttonsLayout->addWidget(m_cancelButton);

    //Layouting form
    QHBoxLayout *exeLayout = new QHBoxLayout;
    exeLayout->addWidget(m_emulatorPathLabel);
    exeLayout->addLayout(exePathLayout, 1);

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(m_emulatorNameLabel);
    nameLayout->addWidget(m_emulatorNameLineEdit, 1);

    QHBoxLayout *emulatedPlatformsLayout = new QHBoxLayout;
    emulatedPlatformsLayout->addWidget(m_emulatorPlatformsLabel);
    emulatedPlatformsLayout->addWidget(m_emulatedPlatforms, 1);

    QHBoxLayout *commandLinePatternLayout = new QHBoxLayout;
    commandLinePatternLayout->addWidget(m_commandLinePatternLabel);
    commandLinePatternLayout->addWidget(m_commandLinePatternText, 1);

    /*m_formLayout->addRow(tr("Emulator Path"), exePathLayout);
    m_formLayout->addRow(tr("Emulator Name"), m_emulatorNameLineEdit);
    m_formLayout->addWidget();
    m_formLayout->addRow(tr("Command Line Pattern"), m_commandLinePatternText);
    m_formLayout->addRow(buttonsLayout);*/

    //m_mainLayout->addLayout(m_formLayout);
    m_mainLayout->addLayout(exeLayout, 1);
    m_mainLayout->addLayout(nameLayout, 1);
    m_mainLayout->addLayout(commandLinePatternLayout, 2);
    m_mainLayout->addLayout(emulatedPlatformsLayout, 4);
    m_mainLayout->addLayout(buttonsLayout, 1);

    this->setLayout(m_mainLayout);
}

void AddEmulatorWindow::initEvents()
{
    connect(m_validateButton, SIGNAL(clicked()), this, SLOT(onValidate()));
    connect(m_browseExeButton, SIGNAL(clicked()), this, SLOT(onBrowseEmulator()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
}

void AddEmulatorWindow::createEmulatorPlatformJunction(int emulatorId, int platformId)
{
    Database database;

    database.open();

    QSqlQuery query(database.db());
    QString sql("INSERT INTO emulators_platforms_junction VALUES(:emulatorId, :platformId)");

    if(!query.prepare(sql)) {
        QString message(tr("An error occured while validating the emulator ") + "\n" + query.lastError().text());
        database.close();
        QMessageBox::critical(this, tr("Error"), message);
    } else {
        query.bindValue(":emulatorId", emulatorId);
        query.bindValue(":platformId", platformId);

        query.exec();
    }
    database.close();
}

void AddEmulatorWindow::populateComboBox()
{
    QList<Platform> platforms = Platforms::getPlatformsList();

    foreach(Platform platform, platforms) {
        m_emulatedPlatforms->addItem(platform.platform_name, platform.platform_id);
    }
}

