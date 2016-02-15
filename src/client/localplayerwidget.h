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
#ifndef LOCALPLAYERWIDGET_H
#define LOCALPLAYERWIDGET_H

#include "parser/parserstructs.h"
#include "playerwidget.h"
#include <ui_localplayerwidget.h>

namespace client {

/**
 * The LocalPlayerWidget instance represent the PlayerWidget that belongs to
 * the local player.
 * @author MacJariel <MacJariel@gmail.com>
 */
class LocalPlayerWidget: public PlayerWidget, public Ui::LocalPlayerWidget
{
Q_OBJECT
public:
    LocalPlayerWidget(QWidget *parent);
    virtual ~LocalPlayerWidget();

    virtual CardListWidget*               hand()            { return mp_hand;             }
    virtual CardListWidget*               table()           { return mp_table;            }
    virtual CharacterWidget*  characterWidget() { return mp_characterWidget;  }
    virtual QLabel*                 playerNameLabel() { return mp_labelPlayerName;  }
    virtual QLabel*                 avatarLabel()     { return mp_labelAvatar;      }
    virtual bool                    isLocalPlayer()   { return 1;                   }

    virtual void enterGameMode(Game* game);

    void setFromPrivateData(const PrivatePlayerData&);
    void setFromContext(const GameContextData&);

protected:
    virtual void clearWidgets();
    virtual void updateWidgets();
    virtual void moveWinnerIcon();
    virtual void setRoleFromPublicData(PlayerRole);
    virtual void setHandSize(int handSize);


private slots:
    void onEndTurnClicked();
    void onPassClicked();
    void onDiscardClicked();

private:
    void updateRoleCardWidget();
};
}
#endif
