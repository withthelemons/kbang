#include "characterbase.h"

#include "playingcard.h"
#include "player.h"
#include "reactioncard.h"
#include "game.h"
#include "gametable.h"
#include "gameexceptions.h"
#include "gameeventmanager.h"
#include "checkdeckresulthandler.h"

CharacterBase::CharacterBase(QObject *parent, CharacterType characterType):
        QObject(parent),
        m_characterType(characterType),
        mp_player(0)
{
}

int CharacterBase::maxLifePoints() const
{
    return 4;
}

void CharacterBase::draw(bool)
{
    mp_player->game()->gameTable().playerDrawFromDeck(mp_player, 2, 0);
}

void CharacterBase::playCard(PlayingCard* card)
{
    card->play();
}

void CharacterBase::playCard(PlayingCard* card, Player* targetPlayer)
{
    card->play(targetPlayer);
}

void CharacterBase::playCard(PlayingCard* card, PlayingCard* targetCard)
{
    card->play(targetCard);
}

void CharacterBase::respondPass(ReactionHandler* reactionHandler)
{
    reactionHandler->respondPass();
}

void CharacterBase::respondCard(ReactionHandler* reactionHandler, PlayingCard* targetCard)
{
    reactionHandler->respondCard(targetCard);
}

void CharacterBase::useAbility()
{
    throw BadUsageException();
}

void CharacterBase::playerDied()
{
}

void CharacterBase::checkDeck(PlayingCard* causedBy,
                              bool (*checkFunc)(PlayingCard*), CheckDeckResultHandler* resultHandler)
{
    PlayingCard* checkedCard = gameTable().checkDeck();
    bool checkResult = (*checkFunc)(checkedCard);
    mp_player->game()->gameEventManager().onPlayerCheckDeck(mp_player, checkedCard, causedBy, checkResult);
    resultHandler->checkResult(checkResult);
}

void CharacterBase::useAbility(Player* targetPlayer)
{
    throw BadUsageException();
}

void CharacterBase::useAbility(QList<PlayingCard*> cards)
{
    throw BadUsageException();
}

void CharacterBase::setPlayer(Player* player)
{
    mp_player = player;
}

void CharacterBase::setCharacterType(CharacterType type)
{
    m_characterType = type;
}

void CharacterBase::notifyAbilityUse()
{
    mp_player->game()->gameEventManager().onPlayerUseAbility(mp_player);
}

GameTable& CharacterBase::gameTable()
{
    return mp_player->game()->gameTable();
}

GameCycle& CharacterBase::gameCycle()
{
    return mp_player->game()->gameCycle();
}
