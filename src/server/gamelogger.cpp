#include "gamelogger.h"
#include "publicplayerview.h"
#include "publicgameview.h"
#include "parser/parserstructs.h"
#include "playingcard.h"

#include <QListIterator>
#include <QDateTime>

using namespace std;

GameLogger::GameLogger()
{
}

void GameLogger::onHandlerRegistered(const PublicGameView* publicGameView, PlayerCtrl* playerCtrl)
{
    Q_UNUSED(playerCtrl);
    mp_publicGameView = publicGameView;
}

void GameLogger::onHandlerUnregistered()
{
    mp_publicGameView = 0;
}

void GameLogger::onGameStarted()
{
    Q_ASSERT(m_logFile.is_open() == 0);


    QString logFileName = QString("game-%1.log").
                          arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    m_logFile.open(qPrintable(logFileName), std::ios::out | std::ios::trunc);



    foreach (const PublicPlayerView* p, mp_publicGameView->publicPlayerList()) {
        QString msg = QString("onPlayerCreated(id=%1, name=\"%2\", role=\"%3\", character=\"%4\")").
                      arg(p->id()).
                      arg(p->name()).
                      arg(playerRoleToString(p->role())).
                      arg(characterTypeToString(p->character()));
        m_logFile << msg.toStdString() << endl;
    }
}

void GameLogger::onGameFinished()
{
    foreach (const PublicPlayerView* p, mp_publicGameView->publicPlayerList()) {
        QString msg = QString("onGameFinished(player=%1, role=\"%2\", winner=\"%3\")").
                      arg(p->id()).
                      arg(playerRoleToString(p->role())).
                      arg(p->isWinner());
        m_logFile << msg.toStdString() << endl;
    }
}


void GameLogger::onPlayerDied(PublicPlayerView& p, PublicPlayerView* causedBy)
{
    QString msg = QString("onPlayerDied(player=%1, causedBy=%2)").
                  arg(p.id()).
                  arg(causedBy->id());
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerDrawFromDeck(PublicPlayerView& p, QList<const PlayingCard*> l, bool)
{
    QString msg = QString("onPlayerDrawFromDeck(player=%1, cards=%2)").
                  arg(p.id()).
                  arg(cardListToString(l));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerDrawFromGraveyard(PublicPlayerView& p, const PlayingCard* c, const PlayingCard*)
{
    QString msg = QString("onPlayerDrawFromGraveyard(player=%1, card=%2)").
                  arg(p.id()).
                  arg(cardToString(c));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerDiscardCard(PublicPlayerView& p, const PlayingCard* c, PocketType)
{
    QString msg = QString("onPlayerDiscardCard(player=%1, card=%2)").
                  arg(p.id()).
                  arg(cardToString(c));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerPlayCard(PublicPlayerView& p, const PlayingCard* c)
{
    QString msg = QString("onPlayerPlayCard(player=%1, card=%2)").
                  arg(p.id()).
                  arg(cardToString(c));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerPlayCard(PublicPlayerView& p, const PlayingCard* c, PublicPlayerView& tp)
{
    QString msg = QString("onPlayerPlayCard(player=%1, card=%2, targetPlayer=%3)").
                  arg(p.id()).
                  arg(cardToString(c)).
                  arg(tp.id());
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerPlayCard(PublicPlayerView& p, const PlayingCard* c, const PlayingCard* tc, PublicPlayerView* tp)
{
    QString msg = QString("onPlayerPlayCard(player=%1, card=%2, targetPlayer=%3, targetCard=%4)").
                  arg(p.id()).
                  arg(cardToString(c)).
                  arg(tp->id()).
                  arg(cardToString(tc));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerPlayCardOnTable(PublicPlayerView& p, const PlayingCard* c, PublicPlayerView& tp)
{
    QString msg = QString("onPlayerPlayCardOnTable(player=%1, card=%2, targetPlayer=%3)").
                  arg(p.id()).
                  arg(cardToString(c)).
                  arg(tp.id());
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPassTableCard(PublicPlayerView& p, const PlayingCard* c, PublicPlayerView& tp)
{
    QString msg = QString("onPassTableCard(player=%1, card=%2, targetPlayer=%3)").
                  arg(p.id()).
                  arg(cardToString(c)).
                  arg(tp.id());
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerRespondWithCard(PublicPlayerView& p, const PlayingCard* c)
{
    QString msg = QString("onPlayerRespondWithCard(player=%1, card=%2)").
                  arg(p.id()).
                  arg(cardToString(c));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerPass(PublicPlayerView& p)
{
    QString msg = QString("onPlayerPass(player=%1)").
                  arg(p.id());
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onDrawIntoSelection(QList<const PlayingCard*> l)
{
    QString msg = QString("onDrawIntoSelection(cards=%1)").
                  arg(cardListToString(l));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerPickFromSelection(PublicPlayerView& p, const PlayingCard* c)
{
    QString msg = QString("onPlayerPickFromSelection(player=%1, card=%2)").
                  arg(p.id()).
                  arg(cardToString(c));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onUndrawFromSelection(const PlayingCard* card)
{
    //m_logFile << "onUndrawFromSelection ()" << endl;
}

void GameLogger::onPlayerCheckDeck(PublicPlayerView& p, const PlayingCard* c, const PlayingCard* cb, bool res)
{
    QString msg = QString("onPlayerCheckDeck(player=%1, checkedCard=%2, causedBy=%3, result=\"%4\")").
                  arg(p.id()).
                  arg(cardToString(c)).
                  arg(cardToString(cb)).
                  arg(res ? "true" : "false");
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onPlayerStealCard(PublicPlayerView& p, PublicPlayerView& tp, PocketType, const PlayingCard* c)
{
    QString msg = QString("onPlayerStealCard(player=%1, targetPlayer=%2, card=%3)").
                  arg(p.id()).
                  arg(tp.id()).
                  arg(cardToString(c));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onCancelCard(PocketType, const PlayingCard* c, PublicPlayerView* tp, PublicPlayerView* p)
{
    QString msg = QString("onCancelCard(player=%1, targetPlayer=%2, card=%3)").
                  arg(p->id()).
                  arg(tp->id()).
                  arg(cardToString(c));
    m_logFile << msg.toStdString() << endl;
}

void GameLogger::onGameContextChange(const GameContextData&)
{
    m_logFile << "onGameContextChange()" << endl;
}

void GameLogger::onLifePointsChange(PublicPlayerView& p, int lifePoints, PublicPlayerView*)
{
    m_logFile << "onLifePointsChange(player=" << p.id() << ", lifePoints=" << lifePoints << ")" << endl;
}

void GameLogger::onDeckRegenerate()
{
    m_logFile << "onDeckRegenerate()" << endl;
}

void GameLogger::onPlayerUseAbility(PublicPlayerView& p)
{
    m_logFile << "onPlayerUseAbility(player=" << p.id() << ")" << endl;
}

QString GameLogger::cardToString(const PlayingCard* card)
{
    if (card == 0) return "card()";
    return "card(" + playingCardTypeToString(card->type()) + "," +
            QString::number(card->rank()) + "," + cardSuitToString(card->suit()) + ")";
}

QString GameLogger::cardListToString(QList<const PlayingCard*> l)
{
    QString res = "[";
    QListIterator<const PlayingCard*> it(l);
    while(it.hasNext()) {
        res += cardToString(it.next());
        if (it.hasNext())
            res += ",";
    }
    res += "]";
    return res;
}
