#ifndef CHARACTERBASE_H
#define CHARACTERBASE_H

#include <QObject>
#include <QList>
#include "parser/parserstructs.h"


class Player;
class PlayingCard;
class ReactionHandler;
class GameTable;
class GameCycle;
class CheckDeckResultHandler;

class CharacterBase : public QObject
{
Q_OBJECT;
public:
    CharacterBase(QObject *parent, CharacterType);

    inline CharacterType characterType() const { return m_characterType; }
    virtual int maxLifePoints() const;

    virtual void draw(bool specialDraw);

    virtual void playCard(PlayingCard* card);
    virtual void playCard(PlayingCard* card, Player* targetPlayer);
    virtual void playCard(PlayingCard* card, PlayingCard* targetCard);

    virtual void respondPass(ReactionHandler*);
    virtual void respondCard(ReactionHandler*, PlayingCard* targetCard);

    virtual void useAbility();
    virtual void useAbility(Player* targetPlayer);
    virtual void useAbility(QList<PlayingCard*> cards);

    virtual void setPlayer(Player* player);
    virtual void playerDied();

    virtual void checkDeck(PlayingCard* causedBy, bool (*checkFunc)(PlayingCard*), CheckDeckResultHandler*);


protected:
    void setCharacterType(CharacterType type);
    void notifyAbilityUse();

    GameTable& gameTable();
    GameCycle& gameCycle();

    CharacterType m_characterType;
    Player*       mp_player;

};

#endif // CHARACTERBASE_H
