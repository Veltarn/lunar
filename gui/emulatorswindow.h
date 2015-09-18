#ifndef EMULATORSWINDOW_H
#define EMULATORSWINDOW_H

#include <QDir>
#include <QDialog>
#include <QHBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QPainter>
#include <QTreeView>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringList>
#include <QDateTime>
#include "tools/processmanager.h"
#include "gui/addemulatorwindow.h"
#include "emulation/emulators.h"
#include "db/database.h"
#include "constants.h"



class EmulatorsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit EmulatorsWindow(QWidget *parent = 0);

signals:

public slots:
    void onAddEmulatorClicked();
    void emulatorSelected(QModelIndex modelIndex);
    void removeEmulator();
    void startEmulator();
protected:
    void paintEvent(QPaintEvent *pe);

private:
    void initGui();
    void initEvents();
    void buildGui();
    void loadEmulators();
    QList<QStandardItem *> createPlatformsSubtree(QList<Platform> &platforms);

    /**
     * @brief Remove the junction between an emulator and it's platforms when the emulator is deleted
     * @param emulatorId
     */
    void removeEmulatorJunction(int emulatorId);
private:
    QHBoxLayout *m_mainLayout;
    QToolBar *m_toolbar;
    QAction *m_addEmulatorAction;
    QAction *m_removeEmulatorAction;
    QAction *m_startEmulatorAction;
    QTreeView *m_emulatorsView;

    QStandardItem *m_selectedItem;
};

#endif // EMULATORSWINDOW_H
