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
#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QLabel>
#include "parser/parserstructs.h"
#include "card.h"

namespace client
{

class CardWidgetFactory;
class GameActionManager;

/**
 * @author MacJariel <MacJariel (at) gmail.com>
 */
class CardWidget: public QLabel
{
Q_OBJECT
public:
    enum Size {
        SIZE_SMALL, SIZE_NORMAL, SIZE_BIG
    };

    CardWidget(QWidget *parent, Card::Type cardType = Card::Playing);
    ~CardWidget();

    inline Card::Type       type()       const { return m_cardType; }
    inline const CardData&  cardData()   const { return m_cardData; }
    inline PocketType       pocketType()     const { return m_pocket; }
    inline int              ownerId()    const { return m_ownerId; }
    inline CharacterType    character()  const { return m_characterType; }

    void setGameActionManager(GameActionManager*);

    void setType(Card::Type);

    void clone(CardWidget*);
    void setCardData(const CardData&);
    void setPocketType(const PocketType&);
    void setOwnerId(int ownerId);

    void setPlayerRole(PlayerRole);
    void setCharacterType(CharacterType);
    void setEmpty();

    void setSize(Size size);
    void validate();


    inline void setShadowMode()    { m_shadowMode = 1; }
    inline void unsetShadowMode()  { m_shadowMode = 0; }

    void setHighlight(bool highlight);


    inline Size  size()  const { return m_size; }
    inline QSize qsize() const { return m_qsize; }
    inline bool  hasHighlight() const { return m_hasHighlight; }
    QPoint center() const;


    inline static QSize smallSize()  { return sm_qsizeSmall;  }
    inline static QSize normalSize() { return sm_qsizeNormal; }
    inline static QSize bigSize()    { return sm_qsizeBig;    }
    static QSize qSize(Size size);
    static int lifeLevel(int lifePoints);

protected:
    virtual void mousePressEvent(QMouseEvent *ev);


private:
    virtual void paintEvent (QPaintEvent *event);

    Card::Type    m_cardType;

    CardData      m_cardData;
    PocketType    m_pocket;
    int           m_ownerId;
    PlayerRole    m_playerRole;
    CharacterType m_characterType;

    Size        m_size;
    QSize       m_qsize;
    bool        m_shadowMode;
    bool        m_hasHighlight;
    bool        m_isEmpty;
    GameActionManager* mp_gameActionManager;

    const static QSize sm_qsizeSmall, sm_qsizeNormal, sm_qsizeBig;
    static int sm_lifeLevels[];
};
}

#endif
