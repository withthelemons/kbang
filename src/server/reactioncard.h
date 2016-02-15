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
#ifndef REACTIONCARD_H
#define REACTIONCARD_H

#include "playingcard.h"
#include "reactionhandler.h"
#include "parser/parserstructs.h"

/**
 * The ReactionCard is the base class for all playing cards, that, when are played,
 * require a reaction of (an)other player(s). This, for example, includes Bang!, Indians,
 * General Store, etc.
 *
 * When the card is played, it can register itself at GameCycle and request gamecycle for
 * a responce from a player. The responce can be either using a card or passing (doing nothing).
 * Thus the ReactionCard childs need to implement these methods:
 *  * virtual void respondPass();
 *  * virtual void respondCard(ReactionCard* targetCard);
 *
 * @author MacJariel <MacJariel@gmail.com>
 */
class ReactionCard: public PlayingCard, public ReactionHandler
{
Q_OBJECT
public:
    ReactionCard(Game *game, int id, PlayingCardType, CardSuit, CardRank);
    //virtual ~ReactionCard();
};

#endif
