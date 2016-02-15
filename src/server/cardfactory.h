#ifndef CARDFACTORY_H
#define CARDFACTORY_H

#include <QMap>
class PlayingCard;
class Game;

/**
 * The CardFactory class is a class that creates cards.
 */
class CardFactory
{
public:
    CardFactory();
    ~CardFactory();
    QMap<int, PlayingCard*> generateCards(Game* game);

private:
    struct CardFactoryImp;
    CardFactoryImp* mp_imp;
};

#endif // CARDFACTORY_H
