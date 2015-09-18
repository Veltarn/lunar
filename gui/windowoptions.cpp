#include "windowoptions.h"
#include "ui_windowoptions.h"

using namespace std;

WindowOptions::WindowOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowOptions),
    m_configurationFile(CONFIGURATION_FILE, QSettings::IniFormat)
{
    ui->setupUi(this);
    this->setMinimumSize(500, 355);
    this->setWindowTitle(tr("Options"));
    this->buildGui();
    this->initEvents();
}

WindowOptions::~WindowOptions()
{
    delete ui;
}

void WindowOptions::buildGui()
{
    m_tabs = new QTabWidget(this);

    m_mainLayout = new QVBoxLayout;
    m_bottomButtonsLayout = new QHBoxLayout;

    m_generalTabWidget = new QWidget(this);
    m_syncTabWidget = new QWidget(this);
    m_apiTabWidget = new QWidget(this);

    m_cancelButton = new Button(tr("Cancel"), this);
    m_validateButton = new Button(tr("Validate"), this);

    /*
     *  Building individuals tabs
     */
    buildGeneralTabWidget();
    buildSaveSyncTabWidget();
    buildApiTabWidget();

    m_tabs->addTab(m_generalTabWidget, tr("General"));
    m_tabs->addTab(m_syncTabWidget, tr("Saves Synchronisation"));
    m_tabs->addTab(m_apiTabWidget, tr("Video Game Databases"));

    m_bottomButtonsLayout->addWidget(m_cancelButton);
    m_bottomButtonsLayout->addWidget(m_validateButton);

    m_mainLayout->addWidget(m_tabs);
    m_mainLayout->addLayout(m_bottomButtonsLayout);

    this->setLayout(m_mainLayout);
}

void WindowOptions::buildGeneralTabWidget()
{
    //Reading options
    m_configurationFile.beginGroup("Slideshow");

    bool isActivatedSlideshow = m_configurationFile.value("activated", true).toBool();
    unsigned int delayBetweenSlides = m_configurationFile.value("delay", 1).toUInt();
    unsigned int cacheDuration = m_configurationFile.value("cacheDuration", 10).toUInt();
    bool neverCleanCache = m_configurationFile.value("neverCleanCache", false).toBool();

    m_configurationFile.endGroup();

    //Getting cache size
    QString cacheSize = m_cache.getCacheSize();

    m_generalTabWidgetMainLayout = new QVBoxLayout;
    m_slideshowGroupLayout = new QFormLayout;

    m_groupSlideshow = new QGroupBox(tr("Activate slideshow"), this);

    m_chkNeverCleanCache    = new QCheckBox(this);
    m_delayBetweenSlides    = new QSpinBox(this);
    m_cacheDuration         = new QSpinBox(this);
    m_cleanCacheButton      = new Button(tr("Clean slideshow cache now"), this);
    m_labelCacheWarning     = new QLabel(this);
    m_cacheSizeLabel        = new QLabel(this);
    m_cacheSizeLabel->setStyleSheet("font-style: italic");

    m_cacheSizeLabel->setText(tr("Cache size") + ": " + cacheSize);

    m_groupSlideshow->setCheckable(true);
    m_groupSlideshow->setToolTip(tr("This option activates a slideshow of game pictures"));

    m_groupSlideshow->setChecked(isActivatedSlideshow);
    m_chkNeverCleanCache->setChecked(neverCleanCache);

    m_delayBetweenSlides->setMinimum(5);
    m_delayBetweenSlides->setMaximum(60);
    m_delayBetweenSlides->setSingleStep(1);
    m_delayBetweenSlides->setValue(delayBetweenSlides);

    m_cacheDuration->setMinimum(0);
    m_cacheDuration->setMaximum(365);
    m_cacheDuration->setSingleStep(1);
    m_cacheDuration->setValue(cacheDuration);

    m_labelCacheWarning->hide();
    m_labelCacheWarning->setWordWrap(true);

    m_slideshowGroupLayout->addRow(tr("Delay in seconds between slides"), m_delayBetweenSlides);
    m_slideshowGroupLayout->addRow(tr("Cache duration in day"), m_cacheDuration);
    m_slideshowGroupLayout->addRow(m_labelCacheWarning);
    m_slideshowGroupLayout->addRow(tr("Never clean the slideshow cache"), m_chkNeverCleanCache);
    m_slideshowGroupLayout->addRow(m_cleanCacheButton);
    m_slideshowGroupLayout->addRow(m_cacheSizeLabel);

    m_groupSlideshow->setLayout(m_slideshowGroupLayout);

    m_generalTabWidgetMainLayout->addWidget(m_groupSlideshow);

    m_generalTabWidget->setLayout(m_generalTabWidgetMainLayout);

    //Manually calling the slots to activate respectives events if necessary
    slotCacheDurationValueChanged(cacheDuration);
    slotNeverCleanCacheClicked(neverCleanCache);

    connect(m_cacheDuration, SIGNAL(valueChanged(int)), this, SLOT(slotCacheDurationValueChanged(int)));
    connect(m_chkNeverCleanCache, SIGNAL(clicked(bool)), this,  SLOT(slotNeverCleanCacheClicked(bool)));
    connect(m_cleanCacheButton, SIGNAL(clicked()), this, SLOT(cleanCache()));
}

void WindowOptions::buildSaveSyncTabWidget()
{
    //Reading configuration file
    m_configurationFile.beginGroup("Sync");

    bool syncEnabled        = m_configurationFile.value("SyncEnabled", true).toBool();
    QString serverAddress   = m_configurationFile.value("ServerAddress", QString()).toString();
    int serverPort          = m_configurationFile.value("port", 21).toInt();
    QString login           = m_configurationFile.value("login", QString()).toString();
    QString password        = m_configurationFile.value("password", QString()).toString();
    int backupNumber        = m_configurationFile.value("backupMax", 1).toInt();

    m_configurationFile.endGroup();

    m_saveSyncTabWidgetFormLayout   = new QFormLayout;
    m_saveSyncTabWidgetMainLayout   = new QVBoxLayout;
    m_serverLineEditsLayout         = new QHBoxLayout;

    m_serverAddress     = new QLineEdit(this);
    m_login             = new QLineEdit(this);
    m_password          = new QLineEdit(this);
    m_serverPort        = new QLineEdit(this);
    m_backupNumber      = new QSpinBox(this);

    m_serverAddress->setEnabled(syncEnabled);
    m_login->setEnabled(syncEnabled);
    m_password->setEnabled(syncEnabled);
    m_serverPort->setEnabled(syncEnabled);
    m_backupNumber->setEnabled(syncEnabled);

    m_serverAddress->setText(serverAddress);
    m_login->setText(login);
    m_password->setText(password);
    m_serverPort->setText(QString::number(serverPort));

    m_password->setEchoMode(QLineEdit::Password);
    m_backupNumber->setSingleStep(1);
    m_backupNumber->setMinimum(1);
    m_backupNumber->setMaximum(150);
    m_backupNumber->setValue(backupNumber);

    m_serverLineEditsLayout->addWidget(new QLabel(tr("Server address")));
    m_serverLineEditsLayout->addWidget(m_serverAddress, 2);
    m_serverLineEditsLayout->addWidget(new QLabel(tr("Server Port")));
    m_serverLineEditsLayout->addWidget(m_serverPort, 1);

    m_chkSyncEnabled = new QCheckBox(tr("Enable save synchronisation"), this);
    m_chkSyncEnabled->setChecked(syncEnabled);
    m_chkSyncEnabled->setCheckable(false);
    m_chkSyncEnabled->setToolTip(tr("Not implemented yet"));

    m_saveSyncTabWidgetFormLayout->addRow(m_chkSyncEnabled);
    m_saveSyncTabWidgetFormLayout->addRow(m_serverLineEditsLayout);
    m_saveSyncTabWidgetFormLayout->addRow(tr("Login"), m_login);
    m_saveSyncTabWidgetFormLayout->addRow(tr("Password"), m_password);
    m_saveSyncTabWidgetFormLayout->addRow(tr("Max number of saves to keep on the server"), m_backupNumber);

    m_saveSyncTabWidgetMainLayout->addLayout(m_saveSyncTabWidgetFormLayout);

    m_syncTabWidget->setLayout(m_saveSyncTabWidgetMainLayout);
}

void WindowOptions::buildApiTabWidget()
{
    m_configurationFile.beginGroup("Database API");
    int currentApi = m_configurationFile.value("API_Index", 0).toInt();
    m_configurationFile.endGroup();
    m_apiTabWidgetMainLayout = new QVBoxLayout;
    m_apiTabWidgetFormLayout = new QFormLayout;

    m_comboBoxDatabaseApiList = new QComboBox(this);
    m_labelDatabaseApiList = new QLabel(this);
    QFont font;
    font.setPointSize(11);
    m_labelDatabaseApiList->setFont(font);
    m_labelDatabaseApiList->setWordWrap(true);

    m_comboBoxDatabaseApiList->addItem(tr("Basic"), "basic");
    m_comboBoxDatabaseApiList->addItem("JeuxVideo.com", "jeuxvideo_dot_com");

    m_comboBoxDatabaseApiList->setCurrentIndex(currentApi);
    this->indexChanged(currentApi);

    m_apiTabWidgetFormLayout->addRow(tr("Database list"), m_comboBoxDatabaseApiList);
    m_apiTabWidgetFormLayout->addWidget(m_labelDatabaseApiList);

    m_apiTabWidgetMainLayout->addLayout(m_apiTabWidgetFormLayout);

    m_apiTabWidget->setLayout(m_apiTabWidgetMainLayout);
}

void WindowOptions::initEvents()
{
    connect(m_validateButton, SIGNAL(clicked()), this, SLOT(validate()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_comboBoxDatabaseApiList, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
    connect(m_chkSyncEnabled, SIGNAL(clicked(bool)), this, SLOT(onChkSyncEnabledClicked(bool)));
}

void WindowOptions::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);

    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
}

void WindowOptions::indexChanged(int index)
{
    switch(index)
    {
        case 0:
        m_labelDatabaseApiList->setText("<p>" + tr("Basic option means that Lunar will use its database to display games informations.</p><p>Unlike network databases, this one doesn't require an internet connection</p>") + "</p>");
        break;
        case 1:
        m_labelDatabaseApiList->setText("<p>" + tr("This API will connect to databases of JeuxVideo.com, a french website. It contains almost every video games since 1995.") + "</p>");
        break;
        default:
            m_labelDatabaseApiList->setText("");
        break;
    }
}

void WindowOptions::validate()
{
    this->saveGeneralOptions();
    this->saveSyncOptions();
    this->saveTabApiOptions();

    this->accept();
}

void WindowOptions::saveGeneralOptions()
{
    bool slideshowActivated = m_groupSlideshow->isChecked();
    int delayBetweenSlides = m_delayBetweenSlides->value();
    int cacheDuration = m_cacheDuration->value();
    bool neverCleanCache = m_chkNeverCleanCache->isChecked();

    m_configurationFile.beginGroup("Slideshow");
    m_configurationFile.setValue("activated", slideshowActivated);
    m_configurationFile.setValue("delay", delayBetweenSlides);
    m_configurationFile.setValue("cacheDuration", cacheDuration);
    m_configurationFile.setValue("neverCleanCache", neverCleanCache);
    m_configurationFile.endGroup();
}

void WindowOptions::saveSyncOptions()
{
    bool syncEnabled = m_chkSyncEnabled->isChecked();
    QString serverAddress = m_serverAddress->text();
    QString login = m_login->text();
    QString pass = m_password->text();
    QString port = m_serverPort->text();
    int backupNumber = m_backupNumber->value();

    m_configurationFile.beginGroup("Sync");

    m_configurationFile.setValue("SyncEnabled", syncEnabled);
    m_configurationFile.setValue("ServerAddress", serverAddress);
    m_configurationFile.setValue("port", port);
    m_configurationFile.setValue("login", login);
    m_configurationFile.setValue("password", pass);
    m_configurationFile.setValue("backupMax", backupNumber);
    m_configurationFile.endGroup();
}

void WindowOptions::saveTabApiOptions()
{
    int currentIndex = m_comboBoxDatabaseApiList->currentIndex();
    QString comboApiValue = m_comboBoxDatabaseApiList->itemData(currentIndex).toString();

    m_configurationFile.beginGroup("Database API");

    m_configurationFile.setValue("API", comboApiValue);
    m_configurationFile.setValue("API_Index", m_comboBoxDatabaseApiList->currentIndex());

    m_configurationFile.endGroup();
}

void WindowOptions::onChkSyncEnabledClicked(bool on)
{
    m_serverAddress->setEnabled(on);
    m_serverPort->setEnabled(on);
    m_login->setEnabled(on);
    m_password->setEnabled(on);
    m_backupNumber->setEnabled(on);
}

void WindowOptions::slotCacheDurationValueChanged(int v)
{
    if(v == 0)
    {
        m_labelCacheWarning->setText("<b>" + tr("The slideshow's cache will never be used. Lunar will have to download pictures every time it request them instead of use a local memory.") + "</b>");
        m_labelCacheWarning->setStyleSheet("color: #e37112");
        m_labelCacheWarning->show();
    }
    else
    {
        m_labelCacheWarning->setText("");
        m_labelCacheWarning->hide();
    }
}

void WindowOptions::slotNeverCleanCacheClicked(bool on)
{
    if(on)
    {
        m_cacheDuration->setEnabled(false);
    }
    else
    {
        m_cacheDuration->setEnabled(true);
    }
}

void WindowOptions::cleanCache()
{
    m_cache.clearCache();
    QString newSize = m_cache.getCacheSize();

    m_cacheSizeLabel->setText(tr("Cache size") + ": " + newSize);
}
