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

#include <QtDebug>
#include "card.h"
#include "config.h"

#include <QPainter>
#include <QFont>

using namespace client;

QMap<PlayingCardType, Card*> Card::sm_playingCards;
QMap<PlayerRole,      Card*> Card::sm_roleCards;
QMap<CharacterType,   Card*> Card::sm_characterCards;

Card::Card(const QString& name, PlayingCardType playingCardType, const QString& imageFileName):
        m_name(name),
        m_type(Card::Playing),
        m_imageFileName(imageFileName)

{
    if (sm_playingCards.contains(playingCardType)) {
        qCritical("Unable to create a playing card. Given type already created.");
        return;
    }
    loadPixmap();
    sm_playingCards[playingCardType] = this;
}

Card::Card(const QString& name, PlayerRole role, const QString& imageFileName):
        m_name(name),
        m_type(Card::Role),
        m_imageFileName(imageFileName)

{
    if (sm_roleCards.contains(role)) {
        qCritical("Unable to create a role card. Given role card already created.");
        return;
    }
    loadPixmap();
    sm_roleCards[role] = this;
}

Card::Card(const QString& name, CharacterType character, const QString& imageFileName):
        m_name(name),
        m_type(Card::Character),
        m_imageFileName(imageFileName)
{
    if (sm_characterCards.contains(character)) {
        qCritical("Unable to create a character card. Given character card already created.");
        return;
    }
    loadPixmap();
    sm_characterCards[character] = this;
}

QPixmap Card::image(const CardSuit& suit, const CardRank& rank) const
{

        QPoint posRank(35, 590);

        QFont font;
        font.setPixelSize(60);
        QPixmap  result = image();
        QPainter painter(&result);

        QString textRank = rankToString(rank);
        QChar   textSuit = suitToChar(suit);
        QColor  suitColor = suitToColor(suit);

        QPainterPath path1;
        QPainterPath path2;
        //timesFont.setStyleStrategy(QFont::ForceOutline);
        path1.addText(posRank, font, textRank);
        QPoint posSuit = posRank + QPoint((int)(font.pixelSize() * 2 * textRank.size() / 3), 0);
        path2.addText(posSuit, font, textSuit);

        painter.setRenderHint(QPainter::Antialiasing);
/*
        painter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));

        painter.setBrush(Qt::black);
        painter.drawPath(path1);
        painter.drawPath(path2);
*/
        painter.setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));

        painter.setBrush(Qt::black);
    painter.drawPath(path1);

        painter.setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));
        painter.setBrush(suitColor);
        painter.drawPath(path2);
        return result;
}


void Card::loadDefaultRuleset()
{
    new Card("Bang!",       CARD_BANG,       "gfx/cards/bang.png");
    new Card("Mancato",     CARD_MISSED,     "gfx/cards/missed.png");
    new Card("Birra",       CARD_BEER,       "gfx/cards/beer.png");
    new Card("Saloon",      CARD_SALOON,     "gfx/cards/saloon.png");
    new Card("Wells Fargo", CARD_WELLSFARGO,  "gfx/cards/wellsfargo.png");
    new Card("Diligenza",   CARD_DILIGENZA,   "gfx/cards/diligenza.png");
    new Card("Emporio",     CARD_GENERALSTORE,"gfx/cards/emporio.png");
    new Card("Panico!",     CARD_PANIC,       "gfx/cards/panico.png");
    new Card("Cat Balou",   CARD_CATBALOU,    "gfx/cards/catbalou.png");
    new Card("Indiani!",    CARD_INDIANS,     "gfx/cards/indians.png");
    new Card("Duello",      CARD_DUEL,        "gfx/cards/duel.png");
    new Card("Gatling",     CARD_GATLING,     "gfx/cards/gatling.png");
    new Card("Mustang",     CARD_MUSTANG,    "gfx/cards/mustang.png");
    new Card("Appaloosa",   CARD_APPALOSSA,  "gfx/cards/appaloosa.png");
    new Card("Barile",      CARD_BARREL,     "gfx/cards/barrel.png");
    new Card("Dinamite",    CARD_DYNAMITE,   "gfx/cards/dynamite.png");
    new Card("Prigione",    CARD_JAIL,       "gfx/cards/jail.png");
    new Card("Volcanic",    CARD_VOLCANIC,   "gfx/cards/volcanic.png");
    new Card("Schofield",   CARD_SCHOFIELD,  "gfx/cards/schofield.png");
    new Card("Winchester",  CARD_WINCHESTER, "gfx/cards/winchester.png");
    new Card("Remington",   CARD_REMINGTON,  "gfx/cards/remington.png");
    new Card("Carabine",    CARD_CARABINE,   "gfx/cards/carabine.png");
    new Card("",            CARD_UNKNOWN,    "gfx/cards/back-playing.png");

    new Card("Bart Cassidy",    CHARACTER_BART_CASSIDY,     "gfx/characters/bart-cassidy.png");
    new Card("Black Jack",      CHARACTER_BLACK_JACK,       "gfx/characters/black-jack.png");
    new Card("Calamity Janet",  CHARACTER_CALAMITY_JANET,   "gfx/characters/calamity-janet.png");
    new Card("El Gringo",       CHARACTER_EL_GRINGO,        "gfx/characters/el-gringo.png");
    new Card("Jesse Jones",     CHARACTER_JESSE_JONES,      "gfx/characters/jesse-jones.png");
    new Card("Jourdonnais",     CHARACTER_JOURDONNAIS,      "gfx/characters/jourdonnais.png");
    new Card("Kit Carlson",     CHARACTER_KIT_CARLSON,      "gfx/characters/kit-carlson.png");
    new Card("Lucky Duke",      CHARACTER_LUCKY_DUKE,       "gfx/characters/lucky-duke.png");
    new Card("Paul Regret",     CHARACTER_PAUL_REGRET,      "gfx/characters/paul-regret.png");
    new Card("Pedro Ramirez",   CHARACTER_PEDRO_RAMIREZ,    "gfx/characters/pedro-ramirez.png");
    new Card("Rose Doolan",     CHARACTER_ROSE_DOOLAN,      "gfx/characters/rose-doolan.png");
    new Card("Sid Ketchum",     CHARACTER_SID_KETCHUM,      "gfx/characters/sid-ketchum.png");
    new Card("Slab the Killer", CHARACTER_SLAB_THE_KILLER,  "gfx/characters/slab-the-killer.png");
    new Card("Suzy Lafayette",  CHARACTER_SUZY_LAFAYETTE,   "gfx/characters/suzy-lafayette.png");
    new Card("Vulture Sam",     CHARACTER_VULTURE_SAM,      "gfx/characters/vulture-sam.png");
    new Card("Willy the Kid",   CHARACTER_WILLY_THE_KID,    "gfx/characters/willy-the-kid.png");
    new Card("",                CHARACTER_UNKNOWN,          "gfx/cards/back-character.png");


    new Card("Sheriff",  ROLE_SHERIFF,    "gfx/cards/sheriff.png");
    new Card("Renegade", ROLE_RENEGADE,   "gfx/cards/renegade.png");
    new Card("Outlaw",   ROLE_OUTLAW,     "gfx/cards/outlaw.png");
    new Card("Deputy",   ROLE_DEPUTY,     "gfx/cards/deputy.png");
    new Card("",         ROLE_UNKNOWN,    "gfx/cards/back-role.png");
}


const Card* Card::findPlayingCard(PlayingCardType id)
{
    return (sm_playingCards.contains(id) ? sm_playingCards[id] : 0);
}

const Card* Card::findRoleCard(PlayerRole id)
{
    return (sm_roleCards.contains(id) ? sm_roleCards[id] : 0);
}

const Card* Card::findCharacterCard(CharacterType id)
{
    return (sm_characterCards.contains(id) ? sm_characterCards[id] : 0);
}

QString Card::rankToString(CardRank rank)
{
    if (rank <= 9) {
        return QString::number(rank);
    } else if (rank == 10) {
        return "10";
    } else if (rank == 11) {
        return "J";
    } else if (rank == 12) {
        return "Q";
    } else if (rank == 13) {
        return "K";
    } else if (rank == 14) {
        return "A";
    }
    return "";
}

QChar Card::suitToChar(CardSuit suit)
{
    switch (suit) {
    case SUIT_CLUBS:    return 0x2663;
    case SUIT_DIAMONDS: return 0x2666;
    case SUIT_HEARTS:   return 0x2665;
    case SUIT_SPADES:   return 0x2660;
    }
    NOT_REACHED();
    return QChar();
}

QColor Card::suitToColor(CardSuit suit)
{
    switch (suit) {
    case SUIT_CLUBS:    return Qt::black;
    case SUIT_DIAMONDS: return Qt::red;
    case SUIT_HEARTS:   return Qt::red;
    case SUIT_SPADES:   return Qt::black;
    }
    NOT_REACHED();
    return QColor();
}

QString Card::suitToColorString(CardSuit suit)
{

    QString color;
    switch (suit) {
    case SUIT_CLUBS:
        color = "black";
        break;
    case SUIT_DIAMONDS:
        color = "red";
        break;
    case SUIT_HEARTS:
        color = "red";
        break;
    case SUIT_SPADES:
        color = "black";
        break;
    }
    return QString("<font color=\"%1\">%2</font>").arg(color).arg(suitToChar(suit));
}


void Card::loadPixmap()
{
    if (!m_image.load(Config::dataPathString() + m_imageFileName)) {
        qWarning(qPrintable(QString("Cannot load pixmap: %1").arg(Config::dataPathString() + m_imageFileName)));
    }
}
