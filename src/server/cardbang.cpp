/***************************************************************************
 *   Copyright (C) 2008 by MacJariel                                       *
 *   echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil"                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "cardbang.h"
#include "player.h"
#include "gamecycle.h"
#include "gameexceptions.h"
#include "gametable.h"
#include "game.h"
#include "cardbarrel.h"

CardBang::CardBang(Game* game, int id, CardSuit cardSuit, CardRank cardRank):
        ReactionCard(game, id, CARD_BANG, cardSuit, cardRank),
        mp_attackedPlayer(0)
{
}


CardBang::~CardBang()
{
}

void CardBang::play(Player *targetPlayer)
{
    gameCycle()->assertTurn();
    assertInHand();

    /* don't allow shoot yourself */
    if (owner() == targetPlayer)
        throw BadTargetPlayerException();

    /* distance check */
    if (game()->getDistance(owner(), targetPlayer) > owner()->weaponRange())
        throw PlayerOutOfRangeException();

    /* one-bang-per-turn check */
    if (!owner()->canPlayBang())
        throw OneBangPerTurnException();

    owner()->onBangPlayed();
    mp_attackingPlayer = owner();
    gameTable()->playerPlayCard(this, targetPlayer);
    m_usedBarrels.clear();
    mp_attackedPlayer = targetPlayer;
    m_missedLeft = mp_attackingPlayer->bangPower();
    game()->gameCycle().setResponseMode(this, targetPlayer);
}

void CardBang::respondPass()
{
    Q_ASSERT(mp_attackedPlayer != 0);
    gameCycle()->unsetResponseMode();
    gameTable()->playerPass(mp_attackedPlayer);
    mp_attackedPlayer->modifyLifePoints(-1, mp_attackingPlayer);

}

void CardBang::respondCard(PlayingCard* targetCard)
{
    switch(targetCard->type()) {
    case CARD_MISSED:
        targetCard->assertInHand();
        game()->gameCycle().unsetResponseMode();
        gameTable()->playerRespondWithCard(targetCard);
        missed();
        return;
    case CARD_BARREL: {
        if (m_usedBarrels.contains(targetCard))
            break;
        targetCard->assertOnTable();
        m_usedBarrels.append(targetCard);
        CardBarrel* barrel = qobject_cast<CardBarrel*>(targetCard);
        barrel->check(this);
        return;
        }
    default:
        break;
    }
    throw BadCardException();
}

void CardBang::checkResult(bool result)
{
    if (result) {
        game()->gameCycle().unsetResponseMode();
        missed();
    }
}

void CardBang::missed()
{
    m_missedLeft--;
    if (m_missedLeft > 0) {
        game()->gameCycle().setResponseMode(this, mp_attackedPlayer);
    }

}


