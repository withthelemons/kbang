#ifndef CARDZOOMWIDGET_H
#define CARDZOOMWIDGET_H

#include <QTime>
#include "cardwidget.h"

namespace client {
class Game;


class CardZoomWidget : public CardWidget
{
Q_OBJECT;
public:
    CardZoomWidget(Game*, CardWidget*);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);

private slots:
    void terminate();

private:
    void onRelease();

    Game*       mp_game;
    QTime       m_time;
    static int  sm_minZoomTime;
};
}
#endif // CARDZOOMWIDGET_H
