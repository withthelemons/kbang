#include <QIODevice>


class MockSocket: public QIODevice
{
    Q_OBJECT

public:
    MockSocket(QObject* parent);
        
    virtual qint64 readData(char* data, qint64 maxSize);
    virtual qint64 writeData(const char* data, qint64 maxSize);
    virtual qint64 bytesAvailable() const;

    virtual bool isSequential() const { return 1; }

private:
    QByteArray m_buffer;

signals:
    void networkOut(QByteArray data);

    
public slots:
    void networkIn(QByteArray data);

};



