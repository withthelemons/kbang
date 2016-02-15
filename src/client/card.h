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
#ifndef CARD_H
#define CARD_H

#include "gameenums.h"
#include "util.h"

#include <QMap>
#include <QString>
#include <QPixmap>


namespace client
{

/**
 * This class is a type of a card in Bang! game. The whole set of Card instances
 * is created during the start of the client. This class manages lifetime of its
 * objects by itself. It's only sufficient to create an instance with \b{new}
 * operator and you should never free any instances with \b{delete} operator by
 * yourself.
 *
 * Cards can be then looked up by their id string. The identification of common cards
 * is specified in KBang specification, so you can use card identification that you
 * got from server (more specifically the parser class) to get the card object.
 *
 * The Card class is almost exclusively used in the CardWidget class.
 * @see client::CardWidget
 *
 * \note In future it may be possible to use different card sets. This will be
 *       implemented by adding a kind of card-set-manager class that will manage
 *       creating the Card objects.
 *
 *
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class Card: private NonCopyable
{
public:
    typedef enum {
        Character, /**< Character card (Susy Lafayete, etc.) */
        Role,      /**< Role card (Sheriff, etc.) */
        Playing    /**< Playing card (Bang!, etc.) */
    } Type;

private:
    /**
     * Constructs a new Card according to given parameters.
     * @param id The unique card identificator.
     * @param name The localized card name (should be used with QObject::tr).
     * @param image The filename of the image, as used in QPixmap constructor.
     */
     Card(const QString& name, PlayingCardType, const QString& imageFileName);
     Card(const QString& name, PlayerRole, const QString& imageFileName);
     Card(const QString& name, CharacterType, const QString& imageFileName);

public:
    inline QString name()  const { return m_name;  } ///< Returns card name.
    inline Type    type()  const { return m_type;  } ///< Returns card type. @see Card::Type
    inline QPixmap image() const { return m_image; } ///< Returns image pixmap.
    QPixmap image(const CardSuit&, const CardRank&) const;

public: /* static */
    static void loadDefaultRuleset();  ///< Loads default ruleset. To be replaced with ruleset manager.

    static const Card* findPlayingCard(PlayingCardType);
    static const Card* findRoleCard(PlayerRole);
    static const Card* findCharacterCard(CharacterType);

    static QString rankToString(CardRank);
    static QChar   suitToChar(CardSuit);
    static QColor  suitToColor(CardSuit);
    static QString suitToColorString(CardSuit);

private:
    void loadPixmap();

    QString m_id;
    QString m_name;
    Type    m_type;
    QPixmap m_image;
    QString m_imageFileName;

    static QMap<PlayingCardType, Card*> sm_playingCards;
    static QMap<PlayerRole,      Card*> sm_roleCards;
    static QMap<CharacterType,   Card*> sm_characterCards;
};
}
#endif
