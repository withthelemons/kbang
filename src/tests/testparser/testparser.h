#include <QtTest/QtTest>
#include "parser/parser.h"

class Parser;
class MockSocket;


class TestParser: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    
    /// TEST CASES
    void serverInfoTest();
    


private:
    MockSocket* mp_clientSocket;
    MockSocket* mp_serverSocket;

    Parser* mp_clientParser;
    Parser* mp_serverParser;
    
    bool m_resultRecieved;

public slots:
    void clientStreamInitialized()
    {
        m_clientStreamInitialized++;
    }
    void serverStreamInitialized()
    {
        m_serverStreamInitialized++;
    }
    void serverinfoRequestRecieved(QueryResult result);
    void serverinfoResultRecieved(const StructServerInfo&);

private:
    int m_clientStreamInitialized, m_serverStreamInitialized;
    QString m_serverName, m_serverDescription;
};
