#include "characterluckyduke.h"
#include "gametable.h"
#include "gamecycle.h"
#include "playingcard.h"
#include "gameexceptions.h"
#include "player.h"
#include "game.h"
#include "gameeventmanager.h"
#include "checkdeckresulthandler.h"

CharacterLuckyDuke::CharacterLuckyDuke(QObject *parent):
        CharacterBase(parent, CHARACTER_LUCKY_DUKE)
{
}


void CharacterLuckyDuke::checkDeck(PlayingCard* causedBy,
                                   bool (*checkFunc)(PlayingCard*),
                                   CheckDeckResultHandler* resultHandler)
{

    mp_causedBy = causedBy;
    notifyAbilityUse();
    gameTable().drawIntoSelection(2);
    mp_resultHandler = resultHandler;
    mp_checkFunc = checkFunc;
    gameCycle().setResponseMode(this, mp_player, 1);
}

void CharacterLuckyDuke::respondCard(PlayingCard* checkedCard)
{
    if (checkedCard->pocket() != POCKET_SELECTION)
        throw BadCardException();
    gameCycle().unsetResponseMode();
    bool checkResult = (*mp_checkFunc)(checkedCard);
    mp_player->game()->gameEventManager().onPlayerCheckDeck(mp_player, checkedCard, mp_causedBy, checkResult);
    gameTable().cancelSelection();
    mp_resultHandler->checkResult(checkResult);
}


