/***************************************************************************
 *   Copyright (C) 2009 by MacJariel                                       *
 *   MacJariel (at) gmail.com                                              *
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

#ifndef GAMEENUMS_H
#define GAMEENUMS_H

#include <QString>

/**
 * @deprecated
 */
enum ActionRequestType {
    REQUEST_DRAW,
    REQUEST_PLAY,
    REQUEST_RESPOND,
    REQUEST_DISCARD
};

enum PlayerRole {
    ROLE_INVALID = 0,
    ROLE_UNKNOWN,
    ROLE_SHERIFF,
    ROLE_DEPUTY,
    ROLE_OUTLAW,
    ROLE_RENEGADE
};

enum PocketType {
    POCKET_INVALID = 0,
    POCKET_DECK,
    POCKET_GRAVEYARD,
    POCKET_HAND,
    POCKET_TABLE,
    POCKET_SELECTION
};

enum PlayingCardType {
    CARD_UNKNOWN = 0,
    CARD_BANG,
    CARD_MISSED,
    CARD_BEER,
    CARD_SALOON,
    CARD_WELLSFARGO,
    CARD_DILIGENZA,
    CARD_GENERALSTORE,
    CARD_PANIC,
    CARD_CATBALOU,
    CARD_INDIANS,
    CARD_DUEL,
    CARD_GATLING,
    CARD_MUSTANG,
    CARD_APPALOSSA,
    CARD_BARREL,
    CARD_DYNAMITE,
    CARD_JAIL,
    CARD_VOLCANIC,
    CARD_SCHOFIELD,
    CARD_REMINGTON,
    CARD_CARABINE,
    CARD_WINCHESTER
};

enum CharacterType {
    CHARACTER_UNKNOWN = 0,
    CHARACTER_BART_CASSIDY,
    CHARACTER_BLACK_JACK,
    CHARACTER_CALAMITY_JANET,
    CHARACTER_EL_GRINGO,
    CHARACTER_JESSE_JONES,
    CHARACTER_JOURDONNAIS,
    CHARACTER_KIT_CARLSON,
    CHARACTER_LUCKY_DUKE,
    CHARACTER_PAUL_REGRET,
    CHARACTER_PEDRO_RAMIREZ,
    CHARACTER_ROSE_DOOLAN,
    CHARACTER_SID_KETCHUM,
    CHARACTER_SLAB_THE_KILLER,
    CHARACTER_SUZY_LAFAYETTE,
    CHARACTER_VULTURE_SAM,
    CHARACTER_WILLY_THE_KID
};

enum ReactionType {
    REACTION_NONE,
    REACTION_BANG,
    REACTION_GATLING,
    REACTION_INDIANS,
    REACTION_DUEL,
    REACTION_GENERALSTORE,
    REACTION_LASTSAVE,
    REACTION_LUCKYDUKE,
    REACTION_KITCARLSON
};

enum ClientType {
    CLIENT_PLAYER,
    CLIENT_SPECTATOR
};

enum CardSuit {
    SUIT_SPADES,
    SUIT_HEARTS,
    SUIT_DIAMONDS,
    SUIT_CLUBS
};

/**
 * Represents the rank of a card. Numeral ranks are represented with the
 * corresponding number, others are represented by following values:
 *  * jack  = 11,
 *  * queen = 12,
 *  * king  = 13,
 *  * ace   = 14
 * This represention allows convenient comparing.
 */
typedef int CardRank;

enum GamePlayState {
    GAMEPLAYSTATE_INVALID = 0,
    GAMEPLAYSTATE_DRAW,
    GAMEPLAYSTATE_TURN,
    GAMEPLAYSTATE_RESPONSE,
    GAMEPLAYSTATE_DISCARD
};

enum GameState {
    GAMESTATE_INVALID = 0,
    GAMESTATE_WAITINGFORPLAYERS,
    GAMESTATE_PLAYING,
    GAMESTATE_FINISHED
};


enum GameMessageType {
    GAMEMESSAGE_INVALID = 0,
    GAMEMESSAGE_GAMESTARTED,
    GAMEMESSAGE_GAMEFINISHED,
    GAMEMESSAGE_PLAYERDRAWFROMDECK,
    GAMEMESSAGE_PLAYERDRAWFROMGRAVEYARD,
    GAMEMESSAGE_PLAYERDISCARDCARD,
    GAMEMESSAGE_PLAYERPLAYCARD,
    GAMEMESSAGE_PLAYERRESPONDWITHCARD,
    GAMEMESSAGE_PLAYERPASS,
    GAMEMESSAGE_PLAYERPICKFROMSELECTION,
    GAMEMESSAGE_PLAYERCHECKDECK,
    GAMEMESSAGE_PLAYERSTEALCARD,
    GAMEMESSAGE_PLAYERCANCELCARD,
    GAMEMESSAGE_DECKREGENERATE,
    GAMEMESSAGE_PLAYERDIED,
};


CardSuit        stringToCardSuit(const QString& s);
QString         cardSuitToString(const CardSuit& suit);
CharacterType   stringToCharacterType(const QString& s);
QString         characterTypeToString(const CharacterType& t);
PlayingCardType stringToPlayingCardType(const QString& s);
QString         playingCardTypeToString(const PlayingCardType& c);
PlayerRole      stringToPlayerRole(const QString& s);
QString         playerRoleToString(const PlayerRole& r);
GamePlayState   stringToGamePlayState(const QString& s);
QString         gamePlayStateToString(const GamePlayState& s);
ReactionType    stringToReactionType(const QString& s);
QString         reactionTypeToString(const ReactionType& r);
PocketType      stringToPocketType(const QString& s);
QString         pocketTypeToString(const PocketType& p);
GameState       stringToGameState(const QString& s);
QString         gameStateToString(const GameState& s);
ClientType      stringToClientType(const QString& s);
QString         clientTypeToString(const ClientType& t);
GameMessageType stringToGameMessageType(const QString& s);
QString         gameMessageTypeToString(const GameMessageType& g);

#endif // GAMEENUMS_H
