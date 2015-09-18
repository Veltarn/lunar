#include "windowexport.h"
#include "ui_windowexport.h"

WindowExport::WindowExport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowExport)
{
    ui->setupUi(this);

    setWindowTitle(tr("Lunar database exportation"));
    setMinimumSize(QSize(500, 160));
    resize(500, 160);

    this->buildGui();
    this->initEvents();
}

WindowExport::~WindowExport()
{
    delete ui;
}

void WindowExport::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);

    QColor bgColor(255, 255, 255);
    QBrush brush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());
}

void WindowExport::exportFile()
{
    bool preserveApi = m_checkPreserveApi->isChecked();
    QString path = m_linePath->text();

    if(!path.isNull())
    {
        Export exportation(this);
        if(exportation.startExport(path, preserveApi))
        {
            this->accept();
        }
    }
    else
    {
        QMessageBox::warning(0, tr("Warning"), tr("No output XML file"));
    }
}

void WindowExport::buildGui()
{
    m_mainLayout = new QVBoxLayout;
    m_formLayout = new QFormLayout;
    m_layoutBrowse = new QHBoxLayout;
    m_layoutButtons = new QHBoxLayout;

    //m_pathLabel = new QLabel(tr("Path"), this);
    m_linePath = new QLineEdit(this);
    m_linePath->setDisabled(true);
    m_checkPreserveApi = new QCheckBox(tr("Preserve API links"), this);

    m_browseButton = new Button(tr("Browse"), this);
    m_exportButton = new Button(tr("Export"), this);
    m_cancelButton = new Button(tr("Cancel"), this);

    m_layoutBrowse->addWidget(m_linePath, 3);
    m_layoutBrowse->addWidget(m_browseButton, 1);

    m_layoutButtons->addWidget(m_cancelButton);
    m_layoutButtons->addWidget(m_exportButton);

    //m_mainLayout->addWidget(m_pathLabel);
    m_formLayout->addRow(tr("Path to XML file: *"), m_layoutBrowse);
    m_formLayout->addRow(m_checkPreserveApi);
    m_mainLayout->addLayout(m_formLayout);
    m_mainLayout->addLayout(m_layoutButtons);

    this->setLayout(m_mainLayout);
}

void WindowExport::initEvents()
{
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_browseButton, SIGNAL(clicked()), this, SLOT(openBrowseWindow()));
    connect(m_exportButton, SIGNAL(clicked()), this, SLOT(exportFile()));
}

void WindowExport::openBrowseWindow()
{
    QString choice = QFileDialog::getSaveFileName(0, tr("Choose a save file"));

    if(!choice.isNull())
    {
        m_linePath->setText(choice);
    }
    else
    {
        m_linePath->setText("");
    }
}
