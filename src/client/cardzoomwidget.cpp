#include "cardzoomwidget.h"
#include "game.h"
#include <QTime>
#include <QTimer>
#include <QMouseEvent>

using namespace client;

int CardZoomWidget::sm_minZoomTime = 500;

CardZoomWidget::CardZoomWidget(Game* game, CardWidget* cardWidget):
        CardWidget(game->mainWidget(), cardWidget->type()),
        mp_game(game)
{
    clone(cardWidget);
    QPoint center = cardWidget->mapTo(mp_game->mainWidget(), cardWidget->center());
    setSize(SIZE_BIG);
    QPoint topLeft = center - QPoint((int)(qsize().width() / 2),
                                     (int)(qsize().height() / 2));
    QPoint bottomRight = topLeft + QPoint(qsize().width(), qsize().height());

    if (topLeft.x() < 0) {
        QPoint move = QPoint(-topLeft.x(), 0);
        topLeft += move;
        bottomRight += move;
    }
    if (topLeft.y() < 0) {
        QPoint move = QPoint(0, -topLeft.y());
        topLeft += move;
        bottomRight += move;
    }
    if (bottomRight.x() > game->mainWidget()->size().width()) {
        QPoint move = QPoint(game->mainWidget()->size().width() - bottomRight.x(), 0);
        topLeft += move;
        bottomRight += move;
    }
    if (bottomRight.y() > game->mainWidget()->size().height()) {
        QPoint move = QPoint(0, game->mainWidget()->size().height() - bottomRight.y());
        topLeft += move;
        bottomRight += move;
    }
    move(topLeft);
    validate();
    raise();
    show();
    grabMouse();
    mp_game->pauseGameEvents();
    m_time.start();
}


void CardZoomWidget::mouseReleaseEvent(QMouseEvent*)
{
    onRelease();
}

void CardZoomWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (!rect().contains(ev->pos()))
        onRelease();
}

void CardZoomWidget::terminate()
{
    releaseMouse();
    mp_game->resumeGameEvents();
    deleteLater();
}

void CardZoomWidget::onRelease()
{
    int timeLeft = sm_minZoomTime - m_time.elapsed();
    if (timeLeft <= 0) {
        terminate();
    } else {
        QTimer::singleShot(timeLeft, this, SLOT(terminate()));
    }
}
