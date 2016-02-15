#include "mocksocket.h"
#include <qdebug.h>

MockSocket::MockSocket(QObject* parent):
    QIODevice(parent)
{
    open(QIODevice::ReadWrite);
}


qint64 MockSocket::readData(char* data, qint64 maxSize)
{
    int size = (m_buffer.size() > maxSize) ? maxSize : m_buffer.size();
    QByteArray d = m_buffer.left(size);
    m_buffer.remove(0, size);
    //qDebug() << this << "readData: " << d;
    qstrncpy(data, d.data(), d.size() + 1);
    return d.size();
}

qint64 MockSocket::writeData(const char* data, qint64 maxSize)
{
    QByteArray d(data);
    int size = (d.size() > maxSize) ? maxSize : m_buffer.size();
//    qDebug("writeData: %s\n", data);
    emit networkOut(data);
    return size;
}

void MockSocket::networkIn(QByteArray data)
{
//    qDebug() << this << "networkIn: " << data;
    m_buffer.append(data);
//    qDebug() << this << "buffer: " << m_buffer;
    emit readyRead();
}


qint64 MockSocket::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}



