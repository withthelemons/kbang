#include "selectplayericonwidget.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QDebug>

using namespace client;

QSize iconSize(64, 64);

SelectPlayerIconWidget::SelectPlayerIconWidget(QWidget* parent)
{
    setCursor(QCursor(Qt::PointingHandCursor));
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setFixedSize(iconSize.width() + 4, iconSize.height() + 4);
}

QString SelectPlayerIconWidget::imageFileName() const
{
    return m_imageFileName;
}

QImage SelectPlayerIconWidget::image() const
{
    return m_image;
}

void SelectPlayerIconWidget::clearImage()
{
    m_imageFileName.clear();
    m_image = QImage();
    setPixmap(QPixmap());
}

void SelectPlayerIconWidget::setImageFileName(QString fileName)
{
    if (fileName.isEmpty()) {
        clearImage();
        return;
    }
    if (!QFile::exists(fileName)) {
        qWarning(qPrintable(QString("Cannot load image file %1: file not found").arg(fileName)));
        clearImage();
        return;
    }
    QImage image;
    if (!image.load(fileName)) {
        qWarning(qPrintable(QString("Cannot load image file %1: unknown format").arg(fileName)));
        clearImage();
        return;
    }
    m_image = image.scaled(iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_imageFileName = fileName;
    setPixmap(QPixmap::fromImage(m_image));
}

void SelectPlayerIconWidget::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton) {
        setImageFileName(QFileDialog::getOpenFileName(this,
            tr("Open Image"), m_imageFileName, tr("Image Files (*.png *.jpg *.bmp)")));
    }
}
