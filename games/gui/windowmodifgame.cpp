#include "windowmodifgame.h"

using namespace std;

WindowModifGame::WindowModifGame(QWidget *parent, int gameId) :
    QDialog(parent),
    m_lastPositionExeWindow(""),
    m_lastPositionSaveWindow(""),
    m_gameId(gameId),
    m_currentRemoteGameId(0),
    m_romArguments(QString())
{
    this->setMinimumSize(500, 300);
    this->resize(500, 300);
    m_api = ApiSelector::getUsedApi();

    this->buildGui();
    this->initEvents();
    m_currentGameType = PC_GAME;

    //If a game have to be edited
    if(m_gameId > 0)
    {
        this->loadGameData();
    }
}

WindowModifGame::~WindowModifGame()
{
}

void WindowModifGame::buildGui()
{
    m_mainLayout                = new QVBoxLayout;
    m_buttonsLayout             = new QHBoxLayout;
    m_formLayout                = new QFormLayout;
    m_savesFormLayout           = new QFormLayout;
    m_exePathLayout             = new QHBoxLayout;
    m_uninstallPathLayout       = new QHBoxLayout;
    m_savePathLayout            = new QHBoxLayout;
    m_gameNameLayout            = new QHBoxLayout;
    m_romOptionsLayout          = new QFormLayout;
    m_commandLineLayout         = new QVBoxLayout;
    m_platformsEmulatorLayout   = new QHBoxLayout;

    m_lineGameName              = new QLineEdit(this);
    m_lineExePath               = new QLineEdit(this);
    m_lineUninstallPath         = new QLineEdit(this);
    //m_lineSavePath              = new QLineEdit(this);

    m_gameNameInfoLabel         = new QLabel(this);
    m_executablePathLabel            = new QLabel(this);

    m_gameTypeBox               = new QComboBox(this);
    m_emulatorCombo             = new QComboBox(this);
    m_commandLineText           = new QTextEdit(this);
    m_commandLinePreview        = new QLabel(this);

    //m_boxSaves                  = new QGroupBox(tr("Saves"), this);
    m_romOptionsBox             = new QGroupBox(tr("Roms options"), this);

    m_btnExePathBrowse          = new Button(tr("Browse"), this);
    m_btnValidate               = new Button(tr("Validate"), this);
    //m_btnSavePathBrowse         = new Button(tr("Browse"), this);
    m_btnUninstallPathBrowse    = new Button(tr("Browse"), this);
    m_btnCancel                 = new Button(tr("Cancel"), this);

    if(m_api != NULL && m_api->isOnlineApi())
    {
        m_btnSearchGame         = new Button(tr("Search"), this);
        m_chkDeleteGameLink     = new QCheckBox(tr("Delete current link"), this);
    }

    m_romOptionsBox->hide();

    m_gameNameInfoLabel->hide();
    m_lineExePath->setEnabled(false);
    //m_lineSavePath->setEnabled(false);
    m_lineUninstallPath->setEnabled(false);
/*
    m_boxSaves->setCheckable(false);
    m_boxSaves->setChecked(false);*/
    //m_btnSavePathBrowse->setEnabled(false);
    m_executablePathLabel->setText(tr("Executable Path") + " ");
    if(m_api != NULL && m_api->isOnlineApi())
    {
        m_btnSearchGame->setEnabled(false);

        //We hide the checkbox if the user try to create a new game
        if(m_gameId == 0)
        {
            m_chkDeleteGameLink->setEnabled(false);
            m_chkDeleteGameLink->hide();
        }
    }

    m_gameTypeBox->addItem(tr("PC Game"), QString("pcGame"));
    m_gameTypeBox->addItem(tr("Emulator Rom"), QString("emulatorRom"));
    this->loadEmulators();


    //Layouting
    //Game name layout (textEdit and if the current api is online, search button)
    m_gameNameLayout->addWidget(m_lineGameName, 3);
    if(m_api != NULL && m_api->isOnlineApi())
    {
        m_gameNameLayout->addWidget(m_btnSearchGame, 1);
    }


    //Création exe file search layout (which is constituted of 2 widgets)
    m_exePathLayout->addWidget(m_lineExePath, 3);
    m_exePathLayout->addWidget(m_btnExePathBrowse, 1);

    m_uninstallPathLayout->addWidget(m_lineUninstallPath, 3);
    m_uninstallPathLayout->addWidget(m_btnUninstallPathBrowse, 1);

    //Création save folder search layout (which is constituted of 2 widgets)
    //m_savePathLayout->addWidget(m_lineSavePath, 3);
    //m_savePathLayout->addWidget(m_btnSavePathBrowse, 1);

    //Putting QLineEdit of game name and exe layout into formLayout
    m_formLayout->addRow(tr("Game name* "), m_gameNameLayout);
    m_formLayout->addRow(tr("Game Type"), m_gameTypeBox);

    m_formLayout->addWidget(m_gameNameInfoLabel);
    if(m_api && m_api->isOnlineApi())
        m_formLayout->addWidget(m_chkDeleteGameLink);
    m_formLayout->addRow(m_executablePathLabel, m_exePathLayout);

    m_formLayout->addRow(tr("Uninstall path (if exists)"), m_uninstallPathLayout);

    //Adding save path layout to a row of save form layout
    m_savesFormLayout->addRow(tr("Game save folder"), m_savePathLayout);

    m_platformsEmulatorLayout->addWidget(m_emulatorCombo);

    // Setting m_savesFormLayout as layout of the box m_boxSaves
    //m_boxSaves->setLayout(m_savesFormLayout);

    m_buttonsLayout->addWidget(m_btnCancel);
    m_buttonsLayout->addWidget(m_btnValidate);

    m_commandLineLayout->addWidget(m_commandLineText, 4);
    m_commandLineLayout->addWidget(m_commandLinePreview, 1);

    m_romOptionsLayout->addRow(tr("Emulator"), m_emulatorCombo);
    m_romOptionsLayout->addRow(tr("Emulator Arguments"), m_commandLineLayout);

    m_romOptionsBox->setLayout(m_romOptionsLayout);

    m_mainLayout->addLayout(m_formLayout, 1);
    //m_mainLayout->addWidget(m_boxSaves, 3);
    m_mainLayout->addWidget(m_romOptionsBox, 3);
    m_mainLayout->addLayout(m_buttonsLayout);

    this->setLayout(m_mainLayout);
}

void WindowModifGame::initEvents()
{
    connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_btnExePathBrowse, SIGNAL(clicked()), this, SLOT(openExeFileSelector()));
    //connect(m_boxSaves, SIGNAL(clicked(bool)), this, SLOT(enableSaveSyncBox(bool)));
    connect(m_btnUninstallPathBrowse, SIGNAL(clicked()), this, SLOT(openUninstallFileSelector()));
    //connect(m_btnSavePathBrowse, SIGNAL(clicked()), this, SLOT(openSavePathSelector()));
    connect(m_btnValidate, SIGNAL(clicked()), this, SLOT(validateForm()));
    connect(m_gameTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGameTypeChanged(int)));
    connect(m_emulatorCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onEmulatorChanged(int)));
    connect(m_commandLineText, SIGNAL(textChanged()), this, SLOT(onArgumentsChanged()));

    if(m_api != NULL && m_api->isOnlineApi())
    {
        connect(m_btnSearchGame, SIGNAL(clicked()), this, SLOT(onBtnSearchClicked()));
        connect(m_lineGameName, SIGNAL(textChanged(QString)), this, SLOT(onLineGameNameChanged(QString)));
        connect(m_chkDeleteGameLink, SIGNAL(clicked(bool)), this, SLOT(onClickCheckBoxDeleteLink(bool)));
    }
}

void WindowModifGame::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);
    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
}

void WindowModifGame::openExeFileSelector()
{
    QString path;

    if(m_currentGameType == PC_GAME) {
#ifdef WINDOWS
        path = QFileDialog::getOpenFileName(this, tr("Select an executable file"), QString(), tr("Executables (*.exe)"));
#else
        path = QFileDialog::getOpenFileName(this, tr("Select an executable file"), QString(), tr("Executables (*.*)"));
#endif
    } else {
        //Assuming it's a rom
        QString filter = QString(tr("ISO Files") + " (*.iso);;" + tr("Binary File") + " (*.bin);;" + tr("ISO Metadata File") + " ( *.cue);;" + tr("All Files") + " (*.*)");
        path = QFileDialog::getOpenFileName(this, tr("Select a rom file"), QString(), filter);
    }

    if(path != "")
    {
        m_lineExePath->setText(path);
        m_lineExePath->setToolTip(path);

        if(m_currentGameType == ROM_GAME) {
            this->updateCommandLine();
        }
    }
}

void WindowModifGame::openUninstallFileSelector()
{
#ifdef WINDOWS
    QString path = QFileDialog::getOpenFileName(this, tr("Select the uninstall file"), QString(), tr("Executables (*.exe)"));
#else
    QString path = QFileDialog::getOpenFileName(this, tr("Select the uninstall file"), QString(), tr("Executables (*.*)"));
#endif

    if(path != "")
    {
        m_lineUninstallPath->setText(path);
        m_lineUninstallPath->setToolTip(path);
    }
}

void WindowModifGame::enableSaveSyncBox(bool on)
{
    /*m_lineSavePath->setText("");
    m_lineSavePath->setToolTip("");

    m_btnSavePathBrowse->setEnabled(on);*/
}

void WindowModifGame::openSavePathSelector()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Select save folder of the game"));

    if(path != "")
    {
        /*m_lineSavePath->setText(path);
        m_lineSavePath->setToolTip(path);*/
    }
}

void WindowModifGame::validateForm()
{
    //Verifying the emptiness of the fields
    if(m_lineGameName->text().isEmpty() || m_lineExePath->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("One of the compulsory fields is empty, please fullfill them"));
        return;
    }

    //Préparing variables
    QString gameName = m_lineGameName->text();
    QString gamePath = m_lineExePath->text();
    QString uninstallPath = m_lineUninstallPath->text();
    //bool allowSaveSync = m_boxSaves->isChecked();
    QString savePath = "";
    int emulatorUsed = m_emulatorCombo->currentData().toInt();
    QString commandLine = m_commandLineText->toPlainText();


    /*if(m_boxSaves->isChecked())
    {
        //If the boxSave has been checked but no save path have been put into the lineEdit
        if(m_lineSavePath->text().isEmpty())
        {
            int response = QMessageBox::question(this, tr("Save Sync desactivation"), tr("You checked the save synchronisation box but didn't fill the <b>Game save folder</b> field.<br />Do you want to do this now ?<br />Clicking on No button will desactivate the save synchronisation"), QMessageBox::Yes | QMessageBox::No);

            if(response == QMessageBox::Yes)
            {
                openSavePathSelector();
                //If the text is still empty
                if(m_lineSavePath->text() == "")
                {
                    //Desactivation of the save sync
                    allowSaveSync = false;
                    m_boxSaves->setChecked(false);
                }
                else
                {
                    savePath = m_lineSavePath->text();
                }
            }
            else
            {
                allowSaveSync = false;
                m_boxSaves->setChecked(false);
            }
        }
        else
        {
            savePath = m_lineSavePath->text();
        }
    }*/

    //If gameId = 0, we are in the case of the addition of a new game
    bool allowSaveSync = false;
    if(m_gameId == 0)
    {
        if(m_currentGameType == PC_GAME) {
            emulatorUsed = -1;
        }

        if(m_game.add(gameName, gamePath, uninstallPath, savePath, allowSaveSync, emulatorUsed, commandLine))
        {
            if(m_api && m_api->isOnlineApi())
                m_game.addApiGameAssociation(m_api->getApiName(), m_gameToLinkWith.getId());
            QMessageBox::information(this, tr("Success"), QString(gameName + " " + tr("has been successfully added")));
        }
    }
    else //Or else we want to update an existing game
    {
        if(m_currentGameType == PC_GAME) {
            emulatorUsed = -1;
        }

        if(m_game.update(m_gameId, gameName, gamePath, uninstallPath, savePath, allowSaveSync, emulatorUsed, commandLine))
        {
            if(m_api && m_api->isOnlineApi())
            {
                if(m_chkDeleteGameLink->isChecked())
                {
                    m_game.deleteApiGameAssociation(m_api->getApiName(), m_gameId, m_currentRemoteGameId);
                }
                else
                {
                    m_game.updateApiGameAssociation(m_api->getApiName(), m_gameId, m_gameToLinkWith.getId(), m_currentRemoteGameId);
                }

            }
            QMessageBox::information(this, tr("Success"), QString(gameName + " " + tr("has been successfully updated")));
        }
    }
    this->accept();
}

void WindowModifGame::loadGameData()
{
    QString apiName = "basic";
    if(m_api && m_api->isOnlineApi())
        apiName = m_api->getApiName();

    QSqlRecord game = m_game.find(m_gameId, apiName);

    qDebug() << game.value("uninstallpath").toString() << endl;

    m_lineGameName->setText(game.value("name").toString());
    m_lineExePath->setText(game.value("exepath").toString());
    m_lineUninstallPath->setText(game.value("uninstallpath").toString());

    int emulatorUsed = game.value("emulator_used").toInt();
    QString commandLine = game.value("command_arguments").toString();

    if(emulatorUsed != -1) {
        int index = m_emulatorCombo->findData(emulatorUsed);
        if(index > -1) {
            m_emulatorCombo->setCurrentIndex(index);
        }
        m_commandLineText->setText(commandLine);
        m_currentGameType = ROM_GAME;
    }

    if(m_api && m_api->isOnlineApi())
    {
        m_gameToLinkWith.setId(game.value("remote_game_id").toInt());
        m_gameToLinkWith.setName(game.value("name").toString());
        m_currentRemoteGameId = game.value("remote_game_id").isNull() ? 0 : game.value("remote_game_id").toInt(0);

        if(!game.value("remote_game_id").isNull())
        {
            m_gameNameInfoLabel->setText("<font color='green'>" + tr("Linked with") + " <i>" + m_gameToLinkWith.getName() + "</i> " + tr("from remote database") + "</font>");
            m_gameNameInfoLabel->show();
        }
        else //If there is no current link to delete
        {
            m_chkDeleteGameLink->hide();
        }
    }

    if(m_currentGameType == ROM_GAME) {
        this->updateCommandLine();
        int index = m_gameTypeBox->findData(QString("emulatorRom"));

        if(index != -1) {
            m_gameTypeBox->setCurrentIndex(index);
        }
    }

    /*m_boxSaves->setChecked(game.value("allowsavesync").toBool());
    if(m_boxSaves->isChecked())
    {
        enableSaveSyncBox(true);
        m_lineSavePath->setText(game.value("savepath").toString());
    }*/
}

void WindowModifGame::loadEmulators()
{
    Emulators emulators;
    m_emulatorsList = emulators.getEmulators();

    foreach(Emulator emulator, m_emulatorsList) {
        m_emulatorCombo->addItem(emulator.emulatorName(), emulator.id());
    }

    int index = m_emulatorCombo->currentIndex();
    this->onEmulatorChanged(index);
}
Emulator WindowModifGame::getEmulator(int id)
{
    foreach(Emulator emulator, m_emulatorsList) {
        if(id == emulator.id()) {
            m_selectedEmulator = emulator;
            return emulator;
        }
    }
    Emulator emulator;
    emulator.setId(-1);
    return emulator;
}

void WindowModifGame::updateCommandLine()
{
    QString isoPath = m_lineExePath->text();
    QString emulatorPath = m_selectedEmulator.exePath();
    m_commandLineReplacer.setCommandLinePattern(m_selectedEmulator.commandLinePattern());

    m_commandLineReplacer.replace("%emulator_path%", "\"" + emulatorPath + "\"");

    if(!isoPath.isEmpty()) {
        m_commandLineReplacer.replace("%rom_path%", "\"" + isoPath + "\"");
    }

    if(!m_romArguments.isEmpty()) {
        m_commandLineReplacer.replace("%optionnal_arguments%", m_romArguments);
    }

    m_commandLinePreview->setText(m_commandLineReplacer.result());
}

void WindowModifGame::onBtnSearchClicked()
{
    connect(m_api, SIGNAL(finishedSearch(ApiGameList)), this, SLOT(onSearchFinished(ApiGameList)));
    m_btnSearchGame->setEnabled(false);
    m_btnSearchGame->setText(tr("Gathering data"));
    QString gameName = m_lineGameName->text();
    m_api->search(gameName);
}

void WindowModifGame::onSearchFinished(ApiGameList agl)
{    
    m_btnSearchGame->setText(tr("Search"));

    DialogSelectGame win(agl, &m_gameToLinkWith, this);
    win.exec();

    if((m_gameToLinkWith.getName() != m_lineGameName->text()) && !win.hasCancelledInput())
    {
        QString msg = tr("Your game name") + " (<i>" + m_lineGameName->text() + "</i>) " + tr("is different that the name which come from the database") + "(<i>" + m_gameToLinkWith.getName() + "</i>).<br />" + tr("Do you want to replace your name by the database name?");
        int rep = QMessageBox::question(this, tr("Game name replacement"), msg, QMessageBox::Yes | QMessageBox::No);

        if(rep == QMessageBox::Yes)
        {
            m_lineGameName->setText(m_gameToLinkWith.getName());
        }
    }

    if(!win.hasCancelledInput())
    {
        m_gameNameInfoLabel->setText("<font color='green'>" + tr("Linked with") + " <i>" + m_gameToLinkWith.getName() + "</i> " + tr("from remote database") + "</font>");
        m_gameNameInfoLabel->show();
    }

    m_api->disconnect(SIGNAL(finishedSearch(ApiGameList)));

    m_btnSearchGame->setEnabled(true);
}

void WindowModifGame::onLineGameNameChanged(QString text)
{
    if(!text.isEmpty())
    {
        m_btnSearchGame->setEnabled(true);
    }
    else
    {
        m_btnSearchGame->setEnabled(false);
    }
}

void WindowModifGame::onClickCheckBoxDeleteLink(bool on)
{
    m_btnSearchGame->setDisabled(on);
}

void WindowModifGame::onGameTypeChanged(int index)
{
    QString data = m_gameTypeBox->itemData(index).toString();

    if(data == "pcGame") {
        m_executablePathLabel->setText(tr("Executable Path") + " ");
        m_btnUninstallPathBrowse->setEnabled(true);
        m_currentGameType = PC_GAME;
        m_romOptionsBox->hide();
    } else if(data == "emulatorRom") {
        m_executablePathLabel->setText(tr("Rom Path") + " ");
        m_btnUninstallPathBrowse->setEnabled(false);
        m_currentGameType = ROM_GAME;
        m_romOptionsBox->show();
    }
}

void WindowModifGame::onEmulatorChanged(int index)
{
    int data = m_emulatorCombo->itemData(index).toInt();

    Emulator emulator = this->getEmulator(data);

    if(emulator.id() != -1) {
        //m_commandLineText->setText(emulator.commandLinePattern());
        m_commandLineReplacer.setCommandLinePattern(emulator.commandLinePattern());
        this->updateCommandLine();
    }
}

void WindowModifGame::onArgumentsChanged()
{
    QString text = m_commandLineText->toPlainText();
    m_romArguments = text;
    this->updateCommandLine();
}
