#ifndef WINDOWEXPORT_H
#define WINDOWEXPORT_H

#include <iostream>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QSize>
#include <QFileDialog>
#include "../../gui/button.h"
#include "export.h"

namespace Ui {
class WindowExport;
}

class WindowExport : public QDialog
{
    Q_OBJECT
    
public:
    explicit WindowExport(QWidget *parent = 0);
    ~WindowExport();

public slots:
    void openBrowseWindow();
    void exportFile();

protected:
    void paintEvent(QPaintEvent *pe);

private:
    void buildGui();
    void initEvents();

private:
    Ui::WindowExport *ui;

    QVBoxLayout *m_mainLayout;
    QFormLayout *m_formLayout;
    QHBoxLayout *m_layoutBrowse;
    QHBoxLayout *m_layoutButtons;

    QLabel *m_pathLabel;

    QLineEdit *m_linePath;

    QCheckBox *m_checkPreserveApi;

    Button *m_browseButton;
    Button *m_exportButton;
    Button *m_cancelButton;

};

#endif // WINDOWEXPORT_H
