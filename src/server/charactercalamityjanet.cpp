#include "charactercalamityjanet.h"
#include "player.h"
#include "playingcard.h"
#include "gameexceptions.h"
#include "cardbang.h"
#include "cardmissed.h"


CharacterCalamityJanet::CharacterCalamityJanet(QObject *parent):
        CharacterBase(parent, CHARACTER_CALAMITY_JANET),
        mp_bang(0),
        mp_missed(0)
{
}


void CharacterCalamityJanet::playCard(PlayingCard* card)
{
    try {
        card->play();
    } catch (BadCardException& e) {
        PlayingCard* swapped = swapCards(card);
        if (swapped) {
            swapped->play();
            notifyAbilityUse();
        } else {
            throw e;
        }
    }
}

void CharacterCalamityJanet::playCard(PlayingCard* card, Player* targetPlayer)
{
    try {
        card->play(targetPlayer);
    } catch (BadUsageException& e) {
        PlayingCard* swapped = swapCards(card);
        if (swapped) {
            swapped->play(targetPlayer);
            notifyAbilityUse();
        } else {
            throw e;
        }
    }
}


void CharacterCalamityJanet::playCard(PlayingCard* card, PlayingCard* targetCard)
{
    try {
        card->play(targetCard);
    } catch (BadCardException& e) {
        PlayingCard* swapped = swapCards(card);
        if (swapped) {
            swapped->play(targetCard);
            notifyAbilityUse();
        } else {
            throw e;
        }
    }
}

void CharacterCalamityJanet::respondCard(ReactionHandler* reactionHandler, PlayingCard* targetCard)
{
    try {
        reactionHandler->respondCard(targetCard);
    } catch (BadCardException& e) {
        PlayingCard* swapped = swapCards(targetCard);
        if (swapped) {
            reactionHandler->respondCard(swapped);
            notifyAbilityUse();
        } else {
            throw e;
        }
    }
}


PlayingCard* CharacterCalamityJanet::swapCards(PlayingCard* card)
{
    switch(card->type()) {
    case CARD_BANG:
        if (!mp_missed)
            mp_missed = new CardMissed(mp_player->game(), 0, SUIT_CLUBS, 2);
        mp_missed->setVirtual(card);
        return mp_missed;
    case CARD_MISSED:
        if (!mp_bang)
            mp_bang = new CardBang(mp_player->game(), 0, SUIT_CLUBS, 2);
        mp_bang->setVirtual(card);
        return mp_bang;
    default:
        break;
    }
    return 0;
}
