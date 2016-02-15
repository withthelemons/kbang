#ifndef SELECTPLAYERICONWIDGET_H
#define SELECTPLAYERICONWIDGET_H

#include <QLabel>
#include <QImage>

namespace client {
class SelectPlayerIconWidget : public QLabel
{
public:
    SelectPlayerIconWidget(QWidget* parent);

    QString imageFileName() const;
    QImage image() const;
    void clearImage();
    void setImageFileName(QString fileName);
    virtual void mousePressEvent(QMouseEvent* ev);

private:
    QString m_imageFileName;
    QImage  m_image;
};
}

#endif // SELECTPLAYERICONWIDGET_H
