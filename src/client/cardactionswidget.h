#ifndef CARDACTIONSWIDGET_H
#define CARDACTIONSWIDGET_H

#include <QWidget>

namespace client
{
class CardWidget;

class CardActionsWidget : public QWidget
{
public:
    CardActionsWidget(QWidget* parent, CardWidget* cardWidget);

private:
    virtual void paintEvent (QPaintEvent *event);

    CardWidget* mp_cardWidget;


};


}
#endif // CARDACTIONSWIDGET_H
