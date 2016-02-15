#ifndef CARDWIDGETSIZEMANAGER_H
#define CARDWIDGETSIZEMANAGER_H

#include <QObject>
#include <QSize>

class QWidget;

namespace client
{

class CardWidgetSizeManager : public QObject {
Q_OBJECT
public:
    enum SizeRole {
        NormalSize,
        ZoomSize
    };


    CardWidgetSizeManager(QObject* parent);
    const QSize& normalSize() const { return m_normalSize; }
    const QSize& zoomSize() const   { return m_zoomSize;   }
    int lifeLevel(int lifePoints) const;

    static QSize initialSize();

signals:
    void cardSizeChanged();

private:
    static int width(int height);

    QSize m_normalSize;
    QSize m_zoomSize;


    static qreal sm_cardRatio;
    static qreal sm_lifePointCardRatio[];
    static int sm_initialHeight;

};

}
#endif // CARDWIDGETSIZEMANAGER_H
