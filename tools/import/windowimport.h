#ifndef WINDOWIMPORT_H
#define WINDOWIMPORT_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QSize>
#include <QFileDialog>
#include <QMessageBox>
#include "../../gui/button.h"
#include "import.h"

namespace Ui {
class WindowImport;
}

class WindowImport : public QDialog
{
    Q_OBJECT
    
public:
    explicit WindowImport(QWidget *parent = 0);
    ~WindowImport();

public slots:
    void onBrowseButtonClick();
    void onImportButtonClick();

protected:
    void paintEvent(QPaintEvent *pe);

private:
    void buildGui();
    void initEvents();
private:
    Ui::WindowImport *ui;

    QHBoxLayout *m_buttonsLayout;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_browseLayout;
    QFormLayout *m_formLayout;

    QLineEdit *m_browseLine;

    Button *m_browseButton;
    Button *m_cancelButton;
    Button *m_importButton;
};

#endif // WINDOWIMPORT_H
