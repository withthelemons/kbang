
#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H


#include "parser/parserstructs.h"
#include <QtDebug>


class GameException
{
public:
    virtual void debug() = 0;
    virtual ~GameException() {}
};

class BadPlayerException: public GameException
{
public:
    BadPlayerException(int playerId): m_playerId(playerId) {}
    int playerId() const { return m_playerId; }
    virtual void debug() {
        qDebug("BadPlayerException");
    }
private:
    int m_playerId;
};


class BadGameStateException: public GameException
{
public:
    virtual void debug() {
        qDebug("BadGameStateException");
    }
};


class BadGamePlayStateException: public GameException
{
public:
    BadGamePlayStateException(GamePlayState current, GamePlayState expected):
            m_current(current), m_expected(expected) {}

    virtual void debug() {
        qDebug(qPrintable(QString("BadGamePlayStateException: expected %1 but current is %1.").arg(
                gamePlayStateToString(m_expected)).arg(gamePlayStateToString(m_current))));
    }
    virtual ~BadGamePlayStateException() {}
private:
    GamePlayState m_current, m_expected;
};

class BadGamePasswordException: public GameException
{
public:
    virtual void debug() {
        qDebug("BadGamePasswordException");
    }
};

class BadPlayerPasswordException: public GameException
{
public:
    virtual void debug() {
        qDebug("BadPlayerPasswordException");
    }
};



class OneBangPerTurnException: public GameException
{
public:
    virtual void debug() {
        qDebug("OneBangPerTurnException");
    }
};

class TwoSameOnTableException: public GameException
{
public:
    virtual void debug() {
        qDebug("TwoSameOnTableException");
    }
};

class BadTargetPlayerException: public GameException
{
public:
    virtual void debug() {
        qDebug("BadTargetPlayerException");
    }
};

class PlayerOutOfRangeException: public GameException
{
public:
    virtual void debug() {
        qDebug("PlayerOutOfRangeException");
    }
};


class BadGameException: public GameException
{
public:
    virtual void debug() {
        qDebug("BadGameException");
    }
};

class TooManyCardsInHandException: public GameException
{
public:
    virtual void debug() {
        qDebug("TooManyCardsInHandException");
    }
};

class BadCardException: public GameException
{
public:
    virtual void debug() {
        qDebug("BadCardException");
    }
};

class BadTargetCardException: public GameException
{
public:
    virtual void debug() {
        qDebug("BadTargetCardException");
    }
};


class BadUsageException: public GameException
{
public:
    virtual void debug() {
        qDebug("BadUsageException");
    }
};

/**
 * @todo add information about predraw card
 */
class BadPredrawException: public GameException
{ 
public:
    virtual void debug() {
        qDebug("BadPredrawException");
    }
};


#endif // GAMEEXCEPTIONS_H
