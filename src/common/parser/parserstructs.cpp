/***************************************************************************
 *   Copyright (C) 2009 by MacJariel                                       *
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

#include "parserstructs.h"
#include "../util.h"
#include "xmlnode.h"
#include <QXmlStreamWriter>
#include <QBuffer>
#include <QtDebug>


QString ServerInfoData::elementName("server-info");
QString PlayerInfoData::elementName("player-info");
QString GameInfoData::elementName("game-info");
QString GameInfoListData::elementName("game-info-list");
QString CreatePlayerData::elementName("create-player");
QString CreateGameData::elementName("create-game");
QString CardData::elementName("card");
QString PublicPlayerData::elementName("public-player");
QString PrivatePlayerData::elementName("private-player");
QString GameContextData::elementName("game-context");
QString GameSyncData::elementName("game-sync");
QString GameMessage::elementName("game-message");
QString CardMovementData::elementName("card-movement");
QString ActionPlayCardData::elementName("play-card");
QString ActionUseAbilityData::elementName("use-ability");



/////////////////////
// local functions //
/////////////////////

void readAvatar(XmlNode* node, QImage& avatar)
{
    QByteArray bytes = QByteArray::fromBase64(node->getFirstChild()->text().toAscii());
    if (!avatar.loadFromData(bytes)) {
        qWarning("Cannot load image from network.");
    }

}

void writeAvatar(QXmlStreamWriter* writer, const QImage& avatar)
{
    if (!avatar.isNull()) {
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        avatar.save(&buffer, "PNG");
        writer->writeStartElement("avatar");
        writer->writeCharacters(bytes.toBase64());
        writer->writeEndElement();
     }
}

////////////////////////
// exported functions //
////////////////////////


void ServerInfoData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    name = node->attribute("name");
    description = node->attribute("description");
}

void ServerInfoData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("name", name);
    writer->writeAttribute("description", description);
    writer->writeEndElement();
}

void PlayerInfoData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    id              = node->attribute("id").toInt();
    name            = node->attribute("name");
    hasPassword     = node->attribute("hasPassword") == "true";
    hasController   = node->attribute("hasController") == "true";
    isAI            = node->attribute("isAI") == "true";
    isAlive         = node->attribute("isAlive") == "true";
}

void PlayerInfoData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("id",            QString::number(id));
    writer->writeAttribute("name",          name);
    writer->writeAttribute("hasPassword",   hasPassword ? "true" : "false");
    writer->writeAttribute("hasController", hasController ? "true" : "false");
    writer->writeAttribute("isAI",          isAI ? "true" : "false");
    writer->writeAttribute("isAlive",       isAlive ? "true" : "false");
    writer->writeEndElement();
}

void GameInfoData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    id                  = node->attribute("id").toInt();
    name                = node->attribute("name");
    description         = node->attribute("description");
    minPlayers          = node->attribute("minPlayers").toInt();
    maxPlayers          = node->attribute("maxPlayers").toInt();
    maxSpectators       = node->attribute("maxSpectators").toInt();
    alivePlayersCnt     = node->attribute("alivePlayersCnt").toInt();
    totalPlayersCnt     = node->attribute("totalPlayersCnt").toInt();
    spectatorsCnt       = node->attribute("spectatorsCnt").toInt();
    AIPlayersCnt        = node->attribute("AIPlayersCnt").toInt();
    hasPlayerPassword   = node->attribute("hasPlayerpassword") == "true";
    hasSpectatorPassword= node->attribute("hasSpectatorPassword") == "true";
    state               = stringToGameState(node->attribute("state"));
    players.clear();
    foreach(XmlNode* child, node->getChildren()) {
        PlayerInfoData playerInfo;
        if (child->name() != PlayerInfoData::elementName) continue;
        playerInfo.read(child);
        players.append(playerInfo);
    }
}

void GameInfoData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("id",                    QString::number(id));
    writer->writeAttribute("name",                  name);
    writer->writeAttribute("description",           description);
    writer->writeAttribute("minPlayers",            QString::number(minPlayers));
    writer->writeAttribute("maxPlayers",            QString::number(maxPlayers));
    writer->writeAttribute("maxSpectators",         QString::number(maxSpectators));
    writer->writeAttribute("alivePlayersCnt",       QString::number(alivePlayersCnt));
    writer->writeAttribute("totalPlayersCnt",       QString::number(totalPlayersCnt));
    writer->writeAttribute("spectatorsCnt",         QString::number(spectatorsCnt));
    writer->writeAttribute("AIPlayersCnt",          QString::number(AIPlayersCnt));
    writer->writeAttribute("hasPlayerPassword",     hasPlayerPassword ? "true" : "false");
    writer->writeAttribute("hasSpectatorPassword",  hasPlayerPassword ? "true" : "false");
    writer->writeAttribute("state",                 gameStateToString(state));
    foreach(const PlayerInfoData& playerInfo, players) {
        playerInfo.write(writer);
    }
    writer->writeEndElement();
}

void GameInfoListData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    clear();
    foreach(XmlNode* child, node->getChildren()) {
        GameInfoData gameInfoData;
        if (child->name() != GameInfoData::elementName)
            continue;
        gameInfoData.read(child);
        append(gameInfoData);
    }
}

void GameInfoListData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    foreach(const GameInfoData& gameInfoData, *this) {
        gameInfoData.write(writer);
    }
    writer->writeEndElement();
}

void CreatePlayerData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    name        = node->attribute("name");
    password    = node->attribute("password");
    XmlNode* avatarNode = node->getFirstChild();
    if (avatarNode && avatarNode->name() == "avatar") {
        readAvatar(avatarNode, avatar);
    }
}

void CreatePlayerData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("name", name);
    if (!password.isNull())
        writer->writeAttribute("password", password);
    writeAvatar(writer, avatar);
    writer->writeEndElement();
}

void CreateGameData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    name                = node->attribute("name");
    description         = node->attribute("description");
    minPlayers          = node->attribute("minPlayers").toInt();
    maxPlayers          = node->attribute("maxPlayers").toInt();
    maxSpectators       = node->attribute("maxSpectators").toInt();
    AIPlayers           = node->attribute("AIPlayers").toInt();
    playerPassword      = node->attribute("playerPassword");
    spectatorPassword   = node->attribute("spectatorPassword");
    flagShufflePlayers  = node->attribute("shufflePlayers") == "true";
}

void CreateGameData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("name",                  name);
    writer->writeAttribute("description",           description);
    writer->writeAttribute("minPlayers",            QString::number(minPlayers));
    writer->writeAttribute("maxPlayers",            QString::number(maxPlayers));
    writer->writeAttribute("maxSpectators",         QString::number(maxSpectators));
    writer->writeAttribute("AIPlayers",             QString::number(AIPlayers));
    if (!playerPassword.isNull())
        writer->writeAttribute("playerPassword",        playerPassword);
    if (!spectatorPassword.isNull())
        writer->writeAttribute("spectatorPassword",     spectatorPassword);
    writer->writeAttribute("shufflePlayers",        flagShufflePlayers ? "true" : "false");
    writer->writeEndElement();
}

void CardData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    id          = node->attribute("id").toInt();
    type        = stringToPlayingCardType(node->attribute("type"));
    suit        = stringToCardSuit(node->attribute("suit"));
    rank        = node->attribute("rank").toInt();
}

void CardData::write(QXmlStreamWriter* writer) const
{

    writer->writeStartElement(elementName);
    writer->writeAttribute("id",        QString::number(id));
    writer->writeAttribute("type",      playingCardTypeToString(type));
    writer->writeAttribute("suit",      cardSuitToString(suit));
    writer->writeAttribute("rank",      QString::number(rank));
    writer->writeEndElement();
}

void PublicPlayerData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    Q_ASSERT(node->getFirstChild()->name() == "cards-table");
    id              = node->attribute("id").toInt();
    name            = node->attribute("name");
    character       = stringToCharacterType(node->attribute("character"));
    lifePoints      = node->attribute("lifePoints").toInt();
    isSheriff       = (node->attribute("isSheriff") == "true");
    handSize        = node->attribute("handSize").toInt();
    hasPassword     = node->attribute("hasPassword") == "true";
    hasController   = node->attribute("hasController") == "true";
    isAI            = node->attribute("isAI") == "true";
    isAlive         = node->attribute("isAlive") == "true";
    isWinner        = node->attribute("isWinner") == "true";
    role            = stringToPlayerRole(node->attribute("role"));
    foreach(XmlNode* child, node->getChildren()) {
        if (child->name() == "cards-table") {
            table.clear();
            foreach(XmlNode* card, child->getChildren()) {
                CardData cardData;
                cardData.read(card);
                table.append(cardData);
            }
        } else if (child->name() == "avatar") {
            readAvatar(child, avatar);
        }
    }
}

void PublicPlayerData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("id",                QString::number(id));
    writer->writeAttribute("name",              name);
    writer->writeAttribute("character",         characterTypeToString(character));
    writer->writeAttribute("lifePoints",        QString::number(lifePoints));
    writer->writeAttribute("isSheriff",         isSheriff ? "true" : "false");
    writer->writeAttribute("handSize",          QString::number(handSize));
    writer->writeAttribute("hasPassword",       hasPassword ? "true" : "false");
    writer->writeAttribute("hasController",     hasController ? "true" : "false");
    writer->writeAttribute("isAI",              isAI ? "true" : "false");
    writer->writeAttribute("isAlive",           isAlive ? "true" : "false");
    writer->writeAttribute("isWinner",          isWinner ? "true" : "false");
    writer->writeAttribute("role",              playerRoleToString(role));
    writer->writeStartElement("cards-table");
    foreach(const CardData& cardData, table)
        cardData.write(writer);
    writer->writeEndElement();
    writeAvatar(writer, avatar);
    writer->writeEndElement();
}

void PrivatePlayerData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    Q_ASSERT(node->getFirstChild()->name() == "cards-hand");
    id      = node->attribute("id").toInt();
    role    = stringToPlayerRole(node->attribute("role"));
    XmlNode* cards = node->getFirstChild();
    hand.clear();
    foreach(XmlNode* card, cards->getChildren()) {
        CardData cardData;
        cardData.read(card);
        hand.append(cardData);
    }
}

void PrivatePlayerData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("id",                QString::number(id));
    writer->writeAttribute("role",              playerRoleToString(role));
    writer->writeStartElement("cards-hand");
    foreach(const CardData& cardData, hand)
        cardData.write(writer);
    writer->writeEndElement();
    writer->writeEndElement();
}

void GameContextData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    currentPlayerId     = node->attribute("currentPlayerId").toInt();
    requestedPlayerId   = node->attribute("requestedPlayerId").toInt();
    turnNumber          = node->attribute("turnNumber").toInt();
    gamePlayState       = stringToGamePlayState(node->attribute("gamePlayState"));
    reactionType        = stringToReactionType(node->attribute("reaction-type"));
    causedBy            = node->attribute("caused-by").toInt();
}

void GameContextData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("currentPlayerId",   QString::number(currentPlayerId));
    writer->writeAttribute("requestedPlayerId", QString::number(requestedPlayerId));
    writer->writeAttribute("turnNumber",        QString::number(turnNumber));
    writer->writeAttribute("gamePlayState",     gamePlayStateToString(gamePlayState));
    writer->writeAttribute("reaction-type",     reactionTypeToString(reactionType));
    writer->writeAttribute("caused-by",         QString::number(causedBy));
    writer->writeEndElement();
}

void GameSyncData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    Q_ASSERT(node->getChildren()[0]->name() == "players");
    id = node->attribute("id").toInt();
    name = node->attribute("name");
    isCreator = node->attribute("is-creator") == "true";
    state = stringToGameState(node->attribute("state"));
    players.clear();
    foreach(XmlNode* player, node->getFirstChild()->getChildren()) {
        PublicPlayerData publicPlayerData;
        publicPlayerData.read(player);
        players.append(publicPlayerData);
    }
    localPlayer.read(node->getChildren()[1]);
    gameContext.read(node->getChildren()[2]);
    graveyard.read(node->getChildren()[3]->getFirstChild());

    selection.clear();
    foreach(XmlNode* cardNode, node->getChildren()[4]->getChildren()) {
        CardData card;
        card.read(cardNode);
        selection.append(card);
    }

}

void GameSyncData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);

    writer->writeAttribute("id", QString::number(id));
    writer->writeAttribute("name", name);
    writer->writeAttribute("is-creator", isCreator ? "true" : "false");
    writer->writeAttribute("state", gameStateToString(state));

    writer->writeStartElement("players");
    foreach (const PublicPlayerData& p, players)
        p.write(writer);
    writer->writeEndElement();
    localPlayer.write(writer);
    gameContext.write(writer);

    writer->writeStartElement("graveyard");
    graveyard.write(writer);
    writer->writeEndElement();

    writer->writeStartElement("selection");
    foreach (const CardData& c, selection)
        c.write(writer);
    writer->writeEndElement();

    writer->writeEndElement();
}

void GameMessage::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    type            = stringToGameMessageType(node->attribute("type"));
    player          = node->attribute("player").toInt();
    targetPlayer    = node->attribute("targetPlayer").toInt();
    causedBy        = node->attribute("causedBy").toInt();
    checkResult     = node->attribute("checkResult") == "true";
    cards.clear();
    foreach(XmlNode* child, node->getChildren()) {
        if (child->name() == "card")
            card.read(child);
        else if (child->name() == "target-card")
            targetCard.read(child->getFirstChild());
        else if (child->name() == "cards") {
            foreach (XmlNode* cardNode, child->getChildren()) {
                CardData cardData;
                cardData.read(cardNode);
                cards.append(cardData);
            }
        }
    }
}

void GameMessage::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("type", gameMessageTypeToString(type));
    if (player)
        writer->writeAttribute("player", QString::number(player));
    if (targetPlayer)
        writer->writeAttribute("targetPlayer", QString::number(targetPlayer));
    if (causedBy)
        writer->writeAttribute("causedBy", QString::number(causedBy));
    if (type == GAMEMESSAGE_PLAYERCHECKDECK)
        writer->writeAttribute("checkResult", checkResult ? "true" : "false");
    if (card.id) {
        card.write(writer);
    }
    if (targetCard.id) {
        writer->writeStartElement("target-card");
        targetCard.write(writer);
        writer->writeEndElement();
    }
    if (cards.size() > 0) {
        writer->writeStartElement("cards");
        foreach(const CardData& cardData, cards)
            cardData.write(writer);
        writer->writeEndElement();
    }
    writer->writeEndElement();
}

void ActionPlayCardData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    playedCardId = node->attribute("id").toInt();
    if (!node->attribute("target-player-id").isNull()) {
        type = PLAYCARD_PLAYER;
        targetPlayerId = node->attribute("target-player-id").toInt();
    } else if (!node->attribute("target-card-id").isNull()) {
        type = PLAYCARD_CARD;
        targetCardId = node->attribute("target-card-id").toInt();
    } else if (!node->attribute("target-hand-id").isNull()) {
        type = PLAYCARD_HAND;
        targetHandId = node->attribute("target-hand-id").toInt();
    } else {
        type = PLAYCARD_SIMPLE;
    }
}

void ActionPlayCardData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("id", QString::number(playedCardId));
    switch(type) {
    case PLAYCARD_SIMPLE:
        break;
    case PLAYCARD_PLAYER:
        writer->writeAttribute("target-player-id", QString::number(targetPlayerId));
        break;
    case PLAYCARD_CARD:
        writer->writeAttribute("target-card-id", QString::number(targetCardId));
        break;
    case PLAYCARD_HAND:
        writer->writeAttribute("target-hand-id", QString::number(targetHandId));
        break;
    }
    writer->writeEndElement();
}

void ActionUseAbilityData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    if (!node->attribute("target-player-id").isNull()) {
        type = TypePlayer;
        targetPlayerId = node->attribute("target-player-id").toInt();
    } else if (node->getChildren().size() == 0) {
        type = TypeSimple;
    } else {
        type = TypeCards;
        foreach (XmlNode* cardNode, node->getChildren()) {
            targetCardsId.append(cardNode->attribute("id").toInt());
        }
    }
}

void ActionUseAbilityData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    switch(type) {
    case TypeSimple:
        break;
    case TypePlayer:
        writer->writeAttribute("target-player-id", QString::number(targetPlayerId));
        break;
    case TypeCards:
        foreach(int cardId, targetCardsId) {
            writer->writeStartElement("card");
            writer->writeAttribute("id", QString::number(cardId));
            writer->writeEndElement();
        }
        break;
    }
    writer->writeEndElement();
}

void CardMovementData::read(XmlNode* node)
{
    Q_ASSERT(node->name() == elementName);
    pocketTypeFrom = stringToPocketType(node->attribute("pocketTypeFrom"));
    pocketTypeTo   = stringToPocketType(node->attribute("pocketTypeTo"));
    playerFrom     = node->attribute("playerFrom").toInt();
    playerTo       = node->attribute("playerTo").toInt();
    foreach(XmlNode* child, node->getChildren()) {
        if (child->name() == "card")
            card.read(child);
        else if (child->name() == "second-card")
            secondCard.read(child->getFirstChild());
    }
}

void CardMovementData::write(QXmlStreamWriter* writer) const
{
    writer->writeStartElement(elementName);
    writer->writeAttribute("pocketTypeFrom", pocketTypeToString(pocketTypeFrom));
    writer->writeAttribute("pocketTypeTo", pocketTypeToString(pocketTypeTo));

    if (playerFrom != 0)
        writer->writeAttribute("playerFrom", QString::number(playerFrom));

    if (playerTo != 0)
        writer->writeAttribute("playerTo", QString::number(playerTo));

    if (card.id != 0)
        card.write(writer);
    if (secondCard.id != 0) {
        writer->writeStartElement("second-card");
        secondCard.write(writer);
        writer->writeEndElement();
    }

    writer->writeEndElement();
}
