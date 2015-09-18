#ifndef WINDOWOPTIONS_H
#define WINDOWOPTIONS_H

#include <iostream>
#include <QDebug>
#include <QDialog>
#include <QPainter>
#include <QTabWidget>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QFont>
#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <QDir>
#include <QFile>
#include <QSettings>
#include "../tools/cache.h"
#include "../constants.h"
#include "button.h"

namespace Ui {
class WindowOptions;
}

class WindowOptions : public QDialog
{
    Q_OBJECT
    
public:
    explicit WindowOptions(QWidget *parent = 0);
    ~WindowOptions();
    
public slots:
    void indexChanged(int index);
    void validate();
    void onChkSyncEnabledClicked(bool on);
    void slotCacheDurationValueChanged(int v);
    void slotNeverCleanCacheClicked(bool on);
    void cleanCache();

protected:
    void paintEvent(QPaintEvent *pe);

private:
    void buildGui();
    void initEvents();

    void buildGeneralTabWidget();
    void buildSaveSyncTabWidget();
    void buildApiTabWidget();

    void saveGeneralOptions();
    void saveSyncOptions();
    void saveTabApiOptions();
private:
    Ui::WindowOptions *ui;

    QTabWidget *m_tabs;

    QWidget *m_generalTabWidget;
    QWidget *m_syncTabWidget;
    QWidget *m_apiTabWidget;

    QCheckBox *m_chkSyncEnabled;
    QCheckBox *m_chkNeverCleanCache;

    QComboBox *m_comboBoxDatabaseApiList; // Used to choose the video game database apis    

    QLabel *m_labelDatabaseApiList; //Used to explain what is the current database selected
    QLabel *m_labelCacheWarning;
    QLabel *m_cacheSizeLabel;

    QLineEdit *m_serverAddress;
    QLineEdit *m_login;
    QLineEdit *m_password;
    QLineEdit *m_serverPort;

    QSpinBox *m_backupNumber;
    QSpinBox *m_delayBetweenSlides;
    QSpinBox *m_cacheDuration;

    QGroupBox *m_groupSlideshow;

    QVBoxLayout *m_mainLayout;
    QVBoxLayout *m_saveSyncTabWidgetMainLayout;
    QHBoxLayout *m_serverLineEditsLayout;
    QFormLayout *m_saveSyncTabWidgetFormLayout;
    QVBoxLayout *m_apiTabWidgetMainLayout;
    QFormLayout *m_apiTabWidgetFormLayout;
    QVBoxLayout *m_generalTabWidgetMainLayout;
    QFormLayout *m_slideshowGroupLayout;
    QHBoxLayout *m_bottomButtonsLayout;

    Button *m_cancelButton;
    Button *m_validateButton;
    Button *m_cleanCacheButton;

    QSettings m_configurationFile;

    Cache m_cache;
};

#endif // WINDOWOPTIONS_H
