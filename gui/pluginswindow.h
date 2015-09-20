#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include <QDialog>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QPainter>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QPluginLoader>

#include "gui/button.h"

#include "core/pluginmanager.h"

class PluginsWindow : public QDialog
{
    Q_OBJECT
public:
    PluginsWindow(QWidget *parent = 0);

public slots:
    void onEnable();
    void onDisable();
    void closeWindow();
    void onValidate();
    void onSelectPlugin(QModelIndex index);
protected:
    void paintEvent(QPaintEvent *);

private:
    void getPluginsData();
    void initGui();
    void buildGui();
    void populateModel();
    void initEvents();
private:
    QTreeView *m_pluginView;
    Button *m_validateButton, *m_closeButton;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_buttonsLayout;

    QStandardItemModel *m_model;
    QToolBar *m_toolbar;
    QAction *m_enablePluginAction;
    QAction *m_disablePluginAction;
    QStandardItem *m_selectedItem;
};

#endif // PLUGINSWINDOW_H
