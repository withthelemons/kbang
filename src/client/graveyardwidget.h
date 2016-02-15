#ifndef GRAVEYARDWIDGET_H
#define GRAVEYARDWIDGET_H

#include "cardpilewidget.h"

namespace client {
class CardWidgetFactory;

/**
 * @author MacJariel <MacJariel@gmail.com>
 */
class GraveyardWidget : public CardPileWidget
{
Q_OBJECT
public:
    GraveyardWidget(QWidget *parent = 0);
    ~GraveyardWidget();

    void init(CardWidgetFactory*);

    virtual CardWidget* pop();
    virtual void push(CardWidget* card);

    void setFirstCard(const CardData&);
    void setSecondCard(const CardData&);

private:
    CardWidget* setCard(const CardData&);

    CardWidget* mp_firstCard;
    CardWidget* mp_secondCard;

    CardWidgetFactory* mp_cardWidgetFactory;
};
}


#endif // GRAVEYARDWIDGET_H
