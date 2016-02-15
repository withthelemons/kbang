#include "characterblackjack.h"
#include "gametable.h"
#include "playingcard.h"

CharacterBlackJack::CharacterBlackJack(QObject* parent):
        CharacterBase(parent, CHARACTER_BLACK_JACK)
{
}

void CharacterBlackJack::draw(bool)
{
    gameTable().playerDrawFromDeck(mp_player, 1, 0);
    QList<const PlayingCard*> cards = gameTable().playerDrawFromDeck(mp_player, 1, 1);
    const PlayingCard* card = cards[0];
    Q_ASSERT(card != 0);
    if (card->suit() == SUIT_HEARTS || card->suit() == SUIT_DIAMONDS) {
        notifyAbilityUse();
        gameTable().playerDrawFromDeck(mp_player, 1, 0);
    }
}
