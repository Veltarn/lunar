#ifndef DIALOGSELECTGAME_H
#define DIALOGSELECTGAME_H

#include <QDebug>
#include <QDialog>
#include <QListView>
#include <QLabel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QColor>

#include "../../gui/button.h"
#include "../apis/apigame.h"

/**
 * @brief Display a list of game from the search method of the api
 */
class DialogSelectGame : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief DialogSelectGame Constructor
     * @param gameList List of games returned by the server
     * @param selectedGame Pointer to ApiGame, it will be used when the user will validate his choice
     * @param parent
     */
    DialogSelectGame(ApiGameList &gameList, ApiGame *selectedGame, QWidget *parent = 0);
    bool hasCancelledInput();
public slots:
    void onClickValidateButton();
    void onClickCancelButton();
protected:
    void paintEvent(QPaintEvent *pe);
private:
    void initDialog();
    void initEvents();
    void initList();
    void layouting();
private:
    QListView *m_listView;
    ApiGameList m_list;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_buttonLayout;
    QLabel *m_infoLabel;

    Button *m_cancelButton;
    Button *m_validateButton;

    ApiGame *m_selectedGame;

    bool m_cancelledInput;
};

#endif // DIALOGSELECTGAME_H
