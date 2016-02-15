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
#ifndef PLAYER_H
#define PLAYER_H

#include <QtCore>
#include <QImage>

#include "publicplayerview.h"
#include "privateplayerview.h"
#include "parser/parserstructs.h"


class PlayingCard;
class CharacterCard;
class WeaponCard;
class PlayerCtrl;
class Game;
class GameEventListener;
class CharacterBase;

/**
 * The Player class represents a Bang! player. The players are created by the Game
 * instance by calling CreateNewPlayer and are destroyed at the end of the game,
 * or before the game starts (typically when the client leaves the game before it
 * starts).
 * Note, that Players cannot be destroyed during the game, thus the disconnection
 * of a client leads either to the premature end of the game, or the current client
 * is replaced with another controller, for example an AI controller or another client.
 *
 * The Player class holds all important player's data, including the player's cards,
 * life points, role, character, etc. Every player has also an unique id (that is also
 * used in network communication with clients).
 *
 * The players can be controlled by controllers. A player controller is a class that
 * implements the GameEventListener interface and acquires the PlayerCtrl instance. A
 * basic controller is the Client class that forms an adapter between remote clients
 * and player controller API.
 *
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class Player : public QObject
{
    Q_OBJECT;
public:
    /**
     * Constructs new player and uses gameEventListener as the controller. The construction
     * of Player objects is the Game class responsibility and other way of creating Players
     * than using Game::createNewPlayer() is not recommended.
     * @param game      the game where the new player is created.
     * @param id        the player's id. It's Game's responsibility to set this unique.
     * @param name      the name of the player
     * @param password  the password for reconnecting into player
     */
    Player(Game* game, int id, const CreatePlayerData&);
    ~Player();
    typedef QList<PlayingCard*> CardList;

  /////////////
 // GETTERS //
/////////////

    /**
     * Returns the id of the player. Zero value is reserved and interpreted
     * as an invalid player.
     */
    inline int                  id()               const { return this ? m_id : 0;     }
    inline PlayerCtrl*          playerCtrl()       const { return mp_playerCtrl;       }
    inline QString              name()             const { return m_name;              }
    inline QString              password()         const { return m_password;          }
    inline QImage               avatar()           const { return m_avatar;            }
    inline Game*                game()             const { return mp_game;             }
    inline int                  lifePoints()       const { return m_lifePoints;        }
    inline int                  maxLifePoints()    const { return m_maxLifePoints;     }
    inline int                  initialCardCount() const { return m_maxLifePoints;     }
    inline int                  handSize()         const { return m_hand.size();       }
    inline CardList             hand()             const { return m_hand;              }
    inline CardList             table()            const { return m_table;             }
    inline CardList             selection()        const { return m_selection;         }
    inline PlayerRole           role()             const { return m_role;              }
    inline CharacterBase*       character()        const { return mp_character;        }
    CharacterType               characterType()    const;
    inline bool                 isAlive()          const { return m_isAlive;           }
    inline bool                 isWinner()         const { return m_isWinner;          }
    inline int                  weaponRange()      const { return m_weaponRange;       }
    inline bool                 hasController()    const { return mp_gameEventListener != 0; }
    inline GameEventListener*    gameEventListener() const { return mp_gameEventListener; }
    inline int                  bangPower()        const { return m_bangPower;         }
    inline PublicPlayerView&    publicView()       const { return m_publicPlayerView;  }
    inline PrivatePlayerView&   privateView()      const { return m_privatePlayerView; }
    bool                        isAI()             const;
    bool                        isPublicRole()     const;

    /**
     * Returns the distance-in modificator. The player is seen by all other
     * players at a distance increased by the amount returned.
     */
    inline int distanceIn() const { return m_distanceIn; }

    /**
     * Returns the distance-out modificator. The player sees all other
     * players at a distance decreased by the amount returned.
     */
    inline int distanceOut() const { return m_distanceOut; }

    /**
     * Returns true, if the player is the creator of the game.
     */
    bool isCreator() const;

    /**
     * Returns true, if the player is on turn.
     */
    bool isOnTurn() const;

    /**
     * Returns true, if the player is requested.
     */
    bool isRequested() const;

    /**
     * Returns true, if the player already has the identical card
     * face up in front of him. This is used to support the
     * "no player can ever have two identical cards face up in
     * front of him" rule.
     * @param card The card that is looked for
     */
    bool hasIdenticalCardOnTable(PlayingCard* card) const;

    /**
     * Returns true, if player can play Bang!. That holds unless the player
     * has already played Bang! in this turn, if player has "Volcanic" on
     * his table, or if player's character is Willy the Kid.
     * @see void Player::onBangPlayed()
     */
    bool canPlayBang() const;

  ///////////////
 // MODIFIERS //
///////////////

    /**
     * Modifies the life points of player by adding it with the argument.
     * In case of life points going out of range, the method sets the
     * minimal or maximal value.
     * If the life points really changed, the method invokes the
     * LifePointsChange event.
     * @param preventDeath set to true to prevent dying (used by instantly
     *                     played beer)
     */
    void modifyLifePoints(int lifePoints, Player* causedBy);
    void lastSaveSuccess(int hitPoints, Player* causedBy);
    void lastSaveFailure(int hitPoints, Player* causedBy);

    void modifyDistanceIn(int delta);
    void modifyDistanceOut(int delta);
    void modifyUnlimitedBangs(int delta);
    void setBangPower(int bangPower);
    void setWeaponRange(int weaponRange);
    void setAlive(bool isAlive);
    void setWinner(bool isWinner);
    void appendCardToHand(PlayingCard* card);
    void appendCardToTable(PlayingCard* card);
    void appendCardToSelection(PlayingCard* card);

    void setPassword(const QString&);

    /**
     * Removes the given card from player's hand. Returns
     * true if player has this card in his hand.
     */
    bool removeCardFromHand(PlayingCard* card);

    PlayingCard* getRandomCardFromHand();
    /**
     * Removes the given card from player's table. Returns
     * true if player has this card in his table.
     */
    bool removeCardFromTable(PlayingCard* card);

    bool removeCardFromSelection(PlayingCard* card);

    void setRoleAndCharacter(const PlayerRole& role, CharacterBase* character);

    void registerPredrawCheck(int checkId);
    void unregisterPredrawCheck(int checkId);
    void predrawCheck(int checkId);

    void update(const CreatePlayerData&);


    /**
     * This method needs to be called after the Bang! card was played.
     * This is necessary to implement the one-bang-per-turn rule.
     * @see bool Player::canPlayBang()
     */
    void onBangPlayed();

    void onTurnStart();

    void registerGameEventListener(GameEventListener*);
    void unregisterGameEventListener();

public slots:
    void checkEmptyHand();

signals:
    void onHit(int lifePoints, Player* causedBy);
    void onEmptyHand();


  ////////////
 // STATIC //
////////////

public:
    /**
     * Converts a PublicPlayerView to its Player instance.
     */
    inline static Player* player(const PublicPlayerView* publicPlayerView) {
        return publicPlayerView->mp_player;
    }

private:
    int                       m_id;
    int                       m_lifePoints;
    int                       m_maxLifePoints;
    QList<PlayingCard*>       m_hand;
    QList<PlayingCard*>       m_table;
    QList<PlayingCard*>       m_selection;
    QString                   m_name;
    QString                   m_password;
    QImage                    m_avatar;
    PlayerRole                m_role;
    CharacterBase*            mp_character;
    bool                      m_isAlive;
    bool                      m_isWinner;
    Game*                     mp_game;
    PlayerCtrl*               mp_playerCtrl;
    GameEventListener*         mp_gameEventListener;

    int                       m_weaponRange;
    int                       m_distanceIn;
    int                       m_distanceOut;
    int                       m_lastBangTurn;
    int                       m_unlimitedBangs;
    int                       m_bangPower;
    int                       m_currentPredraw;

    QList<int>                m_predrawChecks;

    mutable PublicPlayerView   m_publicPlayerView;
    mutable PrivatePlayerView  m_privatePlayerView;
};

#endif
