#ifndef ADDEMULATORWINDOW_H
#define ADDEMULATORWINDOW_H

#include <QDialog>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QLabel>
#include <QDateTime>
#include <QLineEdit>
#include <QTextEdit>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include "gui/qmultiplechoices.h"
#include "emulation/emulatorsknowledgebase.h"
#include "gui/button.h"

#include "db/database.h"
#include "db/platforms.h"
#include "constants.h"

class AddEmulatorWindow : public QDialog
{
    Q_OBJECT
public:
    AddEmulatorWindow(QWidget *parent = 0);
public slots:
    void onBrowseEmulator();
    void closeWindow();
    void onValidate();
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    void initGui();
    void buildGui();
    void initEvents();
    void createEmulatorPlatformJunction(int emulatorId, int platformId);

    void populateComboBox();
private:
    Button *m_validateButton, *m_cancelButton, *m_browseExeButton;
    QLineEdit *m_emulatorNameLineEdit;
    QLineEdit *m_exePathLineEdit;
    QTextEdit *m_commandLinePatternText;
    QMultipleChoices *m_emulatedPlatforms;

    QLabel *m_emulatorPathLabel;
    QLabel *m_emulatorNameLabel;
    QLabel *m_emulatorPlatformsLabel;
    QLabel *m_commandLinePatternLabel;

    QVBoxLayout *m_mainLayout;
    QFormLayout *m_formLayout;
};

#endif // ADDEMULATORWINDOW_H
