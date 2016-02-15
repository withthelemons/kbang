#ifndef CARDWIDGETCLICKHANDLER_H
#define CARDWIDGETCLICKHANDLER_H

#include <QList>

class QString;

namespace client
{

class Game;
class CardWidget;
class PlayerWidget;


class GameActionManager
{
friend class Game;
public:
    bool isClickable(CardWidget*);
    bool onCardClicked(CardWidget*);
    void onCardRightClicked(CardWidget*);
    void onPlayerClicked(PlayerWidget*);
    void onEndTurnClicked();
    void onPassClicked();
    void setDiscardMode(bool inDiscardMode);

private:
    GameActionManager(Game* game);
    void onMainCardClicked(CardWidget*);
    void onCharacterClicked(CardWidget*);

    void selectPlayer(CardWidget* activeCard);
    void selectCards(CardWidget* activeCard, int cardCount = 1);
    void unsetActiveCard();

    void addToSelection(CardWidget* card);
    void removeFromSelection(CardWidget* card);

    void useAbilityWithCards();
    void playWithCards();

    void debug(const QString&);

    enum {
        STATE_MAIN,
        STATE_SELECT_CARDS,
        STATE_SELECT_PLAYER,
        STATE_DISCARD
    } m_state;


    Game*               mp_game;
    CardWidget*         mp_activeCard;
    QList<CardWidget*>  m_cardSelection;
    int                 m_selectionSize;
};
}

#endif // CARDWIDGETCLICKHANDLER_H
