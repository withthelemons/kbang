#include "ioproxy.h"

IOProxy::IOProxy(QObject* parent):
        QIODevice(parent)
{
    open(QIODevice::WriteOnly);
}

IOProxy::~ IOProxy()
{
    close();
}

qint64 IOProxy::readData(char*, qint64)
{
    return 0;
}

qint64 IOProxy::writeData(const char* data, qint64 maxSize)
{
    QByteArray d(data, maxSize);
    emit networkOut(d);
    return d.size();
}
