#include "testparser.h"
#include "mocksocket.h"
#include "parser/parser.h"


void TestParser::initTestCase()
{
    mp_clientSocket = new MockSocket(this);
    mp_serverSocket = new MockSocket(this);

    connect(mp_clientSocket, SIGNAL(networkOut(QByteArray)),
            mp_serverSocket, SLOT(networkIn(QByteArray)), Qt::QueuedConnection);
    connect(mp_serverSocket, SIGNAL(networkOut(QByteArray)),
            mp_clientSocket, SLOT(networkIn(QByteArray)), Qt::QueuedConnection);

    mp_clientParser = new Parser(this, mp_clientSocket);
    mp_serverParser = new Parser(this, mp_serverSocket);

    m_clientStreamInitialized = m_serverStreamInitialized = 0;
    connect(mp_clientParser, SIGNAL(streamInitialized()),
            this, SLOT(clientStreamInitialized()));
    connect(mp_serverParser, SIGNAL(streamInitialized()),
            this, SLOT(serverStreamInitialized()));
    connect(mp_serverParser, SIGNAL(sigQueryServerInfo(QueryResult)),
            this, SLOT(serverinfoRequestRecieved(QueryResult)));
    mp_clientParser->initializeStream();
    QCoreApplication::processEvents();
    QCOMPARE(m_clientStreamInitialized, 1);
    QCOMPARE(m_serverStreamInitialized, 1);
    
    m_serverName = "Testovaci_server";
    m_serverDescription = "Testovaci popis serveru - unicode: čeština podporovaná, esperanto ankaŭ";
}

void TestParser::serverinfoRequestRecieved(QueryResult result)
{
    StructServerInfo x;
    x.name = m_serverName;
    x.description = m_serverDescription;
    result.sendData(x);
}



void TestParser::cleanupTestCase()
{
}

void TestParser::serverInfoTest()
{
    
    
    QueryGet* query = mp_clientParser->queryGet();
    connect(query, SIGNAL(result(const StructServerInfo&)),
            this, SLOT(serverinfoResultRecieved(const StructServerInfo&)));
    query->getServerInfo();
    m_resultRecieved = 0;    
    QCoreApplication::processEvents();
    QCOMPARE(m_resultRecieved, (bool)1);
}

QTEST_MAIN(TestParser)

void TestParser::serverinfoResultRecieved(const StructServerInfo& result)
{
    m_resultRecieved = 1;
    QCOMPARE(result.name, m_serverName);
    QCOMPARE(result.description, m_serverDescription);
}
