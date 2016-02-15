#include "cardfactory.h"
#include "cards.h"

#include <QSet>
#include <cstdlib>

struct CardFactory::CardFactoryImp
{

    void generateCards(Game* game, QMap<int, PlayingCard*>& res)
    {
        QList<PlayingCard*> list;
        //////////
       // BANG //
      //////////
        for (int i = 2; i <= 14; ++i) {
            list.append(new CardBang(game, id(), SUIT_DIAMONDS, i));
        }
        for (int i = 2; i <= 9; ++i) {
            list.append(new CardBang(game, id(), SUIT_CLUBS, i));
        }
        for (int i = 12; i <= 14; ++i) {
            list.append(new CardBang(game, id(), SUIT_HEARTS, i));
        }
        list.append(new CardBang(game, id(), SUIT_SPADES, 14));


        ////////////
       // MISSED //
      ////////////
        for (int i = 2; i <= 8; ++i) {
            list.append(new CardMissed(game, id(), SUIT_SPADES, i));
        }
        for (int i = 10; i <= 14; ++i) {
            list.append(new CardMissed(game, id(), SUIT_CLUBS, i));
        }

        ///////////////////
       // BEER + SALOON //
      ///////////////////
        for (int i = 6; i <= 11; ++i) {
            list.append(new CardBeer(game, id(), SUIT_HEARTS, i, 0));
        }
        list.append(new CardBeer(game, id(), SUIT_HEARTS, 5, 1));

        ////////////
       // HORSES //
      ////////////
        list.append(new CardHorse(game, id(), CardHorse::Appaloosa, SUIT_SPADES, 14));
        list.append(new CardHorse(game, id(), CardHorse::Mustang,   SUIT_HEARTS, 8));
        list.append(new CardHorse(game, id(), CardHorse::Mustang,   SUIT_HEARTS, 9));

        /////////////
       // WEAPONS //
      /////////////
        list.append(new WeaponCard(game, id(), 1, SUIT_SPADES, 10));
        list.append(new WeaponCard(game, id(), 1, SUIT_CLUBS,  10));
        list.append(new WeaponCard(game, id(), 2, SUIT_CLUBS,  11));
        list.append(new WeaponCard(game, id(), 2, SUIT_CLUBS,  12));
        list.append(new WeaponCard(game, id(), 2, SUIT_SPADES, 13));
        list.append(new WeaponCard(game, id(), 3, SUIT_CLUBS,  13));
        list.append(new WeaponCard(game, id(), 4, SUIT_CLUBS,  14));
        list.append(new WeaponCard(game, id(), 5, SUIT_SPADES, 8));

        ////////////////
       // DRAW CARDS //
      ////////////////
        list.append(new CardDrawCards(game, id(), CardDrawCards::Diligenza,  SUIT_SPADES,  9));
        list.append(new CardDrawCards(game, id(), CardDrawCards::Diligenza,  SUIT_SPADES,  9));
        list.append(new CardDrawCards(game, id(), CardDrawCards::WellsFargo, SUIT_HEARTS,  3));

        ////////////////
       // MULTISHOOT //
      ////////////////
        list.append(new CardMultiShoot(game, id(), CardMultiShoot::Indians, SUIT_DIAMONDS,  13));
        list.append(new CardMultiShoot(game, id(), CardMultiShoot::Indians, SUIT_DIAMONDS,  14));
        list.append(new CardMultiShoot(game, id(), CardMultiShoot::Gatling, SUIT_HEARTS,    10));

        //////////
       // DUEL //
      //////////
        list.append(new CardDuel(game, id(), SUIT_CLUBS,    8 ));
        list.append(new CardDuel(game, id(), SUIT_SPADES,   11));
        list.append(new CardDuel(game, id(), SUIT_DIAMONDS, 12));


        list.append(new CardGeneralStore(game, id(), SUIT_SPADES, 12));
        list.append(new CardGeneralStore(game, id(), SUIT_CLUBS, 9));

        list.append(new CardJail(game, id(), SUIT_SPADES, 10));
        list.append(new CardJail(game, id(), SUIT_SPADES, 11));
        list.append(new CardJail(game, id(), SUIT_HEARTS, 4));

        list.append(new CardDynamite(game, id(), SUIT_HEARTS, 2));

        list.append(new CardTaker(game, id(), CardTaker::Panic, SUIT_DIAMONDS, 8 ));
        list.append(new CardTaker(game, id(), CardTaker::Panic, SUIT_HEARTS,   11));
        list.append(new CardTaker(game, id(), CardTaker::Panic, SUIT_HEARTS,   12));
        list.append(new CardTaker(game, id(), CardTaker::Panic, SUIT_HEARTS,   14));

        list.append(new CardTaker(game, id(), CardTaker::CatBalou, SUIT_DIAMONDS, 9 ));
        list.append(new CardTaker(game, id(), CardTaker::CatBalou, SUIT_DIAMONDS, 10));
        list.append(new CardTaker(game, id(), CardTaker::CatBalou, SUIT_DIAMONDS, 11));
        list.append(new CardTaker(game, id(), CardTaker::CatBalou, SUIT_HEARTS,   13));

        list.append(new CardBarrel(game, id(), SUIT_SPADES, 12));
        list.append(new CardBarrel(game, id(), SUIT_SPADES, 13));

        foreach(PlayingCard* card, list) {
            res[card->id()] = card;
        }
    }


    int id()
    {
        int cardId;
        do {
            cardId = (int)qrand();
        } while(m_ids.contains(cardId));
        m_ids.insert(cardId);
        return cardId;
    }

    QSet<int>   m_ids;
};

CardFactory::CardFactory()
{
    mp_imp = new CardFactoryImp();
}

CardFactory::~CardFactory()
{
    delete mp_imp;
}

QMap<int, PlayingCard*> CardFactory::generateCards(Game* game)
{
    QMap<int, PlayingCard*> res;
    mp_imp->generateCards(game, res);
    return res;

}
