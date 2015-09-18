#include "windowimport.h"
#include "ui_windowimport.h"

WindowImport::WindowImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowImport)
{
    ui->setupUi(this);

    setWindowTitle(tr("Import file into Lunar database"));
    setFixedSize(QSize(500, 154));

    buildGui();
    initEvents();
}

WindowImport::~WindowImport()
{
    delete ui;
}

void WindowImport::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);

    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
}

void WindowImport::buildGui()
{
    m_mainLayout    = new QVBoxLayout;
    m_buttonsLayout = new QHBoxLayout;
    m_formLayout    = new QFormLayout;
    m_browseLayout  = new QHBoxLayout;

    m_browseLine    = new QLineEdit(this);

    m_browseButton  = new Button(tr("Browse"), this);
    m_cancelButton  = new Button(tr("Cancel"), this);
    m_importButton  = new Button(tr("Import"), this);

    m_browseLine->setDisabled(true);

    m_buttonsLayout->addWidget(m_cancelButton);
    m_buttonsLayout->addWidget(m_importButton);

    m_browseLayout->addWidget(m_browseLine, 3);
    m_browseLayout->addWidget(m_browseButton, 1);

    m_formLayout->addRow(tr("XML file"), m_browseLayout);

    m_mainLayout->addLayout(m_formLayout);
    m_mainLayout->addLayout(m_buttonsLayout);

    setLayout(m_mainLayout);
}

void WindowImport::initEvents()
{
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_browseButton, SIGNAL(clicked()), this, SLOT(onBrowseButtonClick()));
    connect(m_importButton, SIGNAL(clicked()), this, SLOT(onImportButtonClick()));
}

void WindowImport::onBrowseButtonClick()
{
    QString path = QFileDialog::getOpenFileName(0, tr("Select a file to import into Lunar"), QString(), QString("Xml Files(*.xml)"));

    if(!path.isEmpty())
    {
        m_browseLine->setText(path);
    }
    else
    {
        m_browseLine->setText("");
    }
}

void WindowImport::onImportButtonClick()
{
    if(m_browseLine->text().isEmpty())
    {
        QMessageBox::warning(0, tr("Cannot import file"), tr("Unable to import file.<br/>No file selected"));
    }

    Import import(this);

    if(!import.startImport(m_browseLine->text()))
    {
        QMessageBox::warning(0, tr("Error"), tr("An error has occured during importation process"));
    }
    else
    {
        QMessageBox::information(0, tr("Success"), tr("Importation has succeed"));
        this->accept();
    }
}
