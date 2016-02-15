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
#ifndef WEAPONCARD_H
#define WEAPONCARD_H

#include "tablecard.h"
#include "parser/parserstructs.h"

/**
 * The WeaponCard represents any weapon card.
 * @author MacJariel <MacJariel@gmail.com>
 */
class WeaponCard: public TableCard
{
Q_OBJECT
public:
    /**
     * Constructs a weapon card. The concrete card is created according to the range
     * parameter:
     *  * 1 - Volcanic
     *  * 2 - Schofield
     *  * 3 - Remington
     *  * 4 - Carabine
     *  * 5 - Winchester
     */
    WeaponCard(Game *game, int id, int range, CardSuit, CardRank);
    virtual ~WeaponCard();

    virtual void play();

    virtual void registerPlayer(Player* player);
    virtual void unregisterPlayer(Player* player);


private:
    int m_range;
};

#endif
