#ifndef CARDMULTISHOOT_H
#define CARDMULTISHOOT_H

#include "reactioncard.h"
#include "checkdeckresulthandler.h"

class Player;

class CardMultiShoot : public ReactionCard, public CheckDeckResultHandler
{
Q_OBJECT;
public:
    enum Type {
        Indians,
        Gatling
    };
    CardMultiShoot(Game* game, int id, Type type, CardSuit, CardRank);
    ~CardMultiShoot();
    virtual void play();

    virtual void respondPass();
    virtual void respondCard(PlayingCard* targetCard);

    virtual void checkResult(bool result);

    virtual ReactionType reactionType() const;
    virtual Player* causedBy() const { return mp_shootingPlayer; }

private:
    void requestNext();

    Type    m_type;
    Player* mp_shootingPlayer;
    Player* mp_requestedPlayer;
    QList<PlayingCard*> m_usedBarrels;
};


#endif // CARDMULTISHOOT_H
