/***************************************************************************
 *   Copyright (C) 2008 by MacJariel                                       *
 *   echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil"                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "servertester.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ServerTester serverTester;
    return app.exec();
}

QLayout* ShortcutButton::smp_layout = 0;
QTextEdit* ShortcutButton::smp_textEdit = 0;
QPushButton* ShortcutButton::smp_sendButton = 0;

ShortcutButton::ShortcutButton(QString title, QString content):
m_title(title), m_content(content)
{
    Q_ASSERT(smp_layout);
    this->setText(title);
    smp_layout->addWidget(this);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(this, SIGNAL(clicked()),
            this, SLOT(updateTextEdit()));
}

void ShortcutButton::updateTextEdit()
{
    Q_ASSERT(smp_textEdit);
    Q_ASSERT(smp_sendButton);
    smp_textEdit->setText(m_content);
    smp_sendButton->setFocus(Qt::OtherFocusReason);
}

void ShortcutButton::init(QLayout* layout,
                          QTextEdit* textEdit,
                          QPushButton* sendButton)
{
    smp_layout = layout;
    smp_textEdit = textEdit;
    smp_sendButton = sendButton;
}

void ServerTester::initButtons()
{
    ShortcutButton::init(mp_layoutButtons, mp_textEditInputXml, mp_pushButtonSendXml);
    new ShortcutButton("<stream>", "<stream>");
    new ShortcutButton("</stream>", "</stream>");
    new ShortcutButton("List Games", "<query type=\"get\" id=\"list-games\">\n  <gamelist />\n</query>");
    new ShortcutButton("Get Game", "<query type=\"get\" id=\"get-game\">\n  <game id=\"1\" />\n</query>");

    new ShortcutButton("Create Game","<action>\n  <create-game name=\"The Game\" desc=\"The Game Description\" minplayers=\"3\" maxplayers=\"7\" maxobservers=\"99\">\n    <player name=\"Tester\" password=\"1234\" />\n  </create-game>\n</action>");
    new ShortcutButton("Join Game", "<action>\n  <join-game id=\"1\">\n    <player name=\"Tester\" password=\"1234\" />\n  </join-game>\n</action>");
    new ShortcutButton("Leave Game", "<action>\n  <leave-game /\">\n</action>");
}



ServerTester::ServerTester(QWidget *parent):
QWidget(parent),
m_connected(0)
{
    this->setWindowTitle("KBang Server Tester");
    mp_lineEditAddress = new QLineEdit;
    mp_lineEditAddress->setText("127.0.0.1");
    mp_lineEditAddress->setMaxLength(15);
    mp_lineEditAddress->setInputMask("009.009.009.009; ");
    mp_lineEditAddress->setMaximumWidth(100);
    mp_lineEditPort = new QLineEdit;
    mp_lineEditPort->setText("6543");
    mp_lineEditPort->setMaxLength(6);
    mp_lineEditPort->setMaximumWidth(40);
    QValidator *portValidator = new QIntValidator(1, 65535, this);
    mp_lineEditPort->setValidator(portValidator);

    mp_pushButtonConnect = new QPushButton;
    mp_pushButtonConnect->setText("Connect");
    mp_pushButtonConnect->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    mp_layoutButtons = new QGridLayout;

    mp_layoutConnect = new QHBoxLayout;
    mp_layoutConnect->addWidget(mp_lineEditAddress);
    mp_layoutConnect->addWidget(mp_lineEditPort);
    mp_layoutConnect->addWidget(mp_pushButtonConnect);


    mp_textEditViewXml = new QTextEdit;
    mp_textEditViewXml->setReadOnly(1);
    mp_textEditViewXml->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mp_textEditInputXml = new QTextEdit;
    mp_textEditInputXml->setMaximumHeight(100);
    mp_pushButtonSendXml = new QPushButton;
    mp_pushButtonSendXml->setText("Send");
    mp_pushButtonSendXml->setEnabled(0);

    mp_layoutXmlInput = new QHBoxLayout;
    mp_layoutXmlInput->addWidget(mp_textEditInputXml);
    mp_layoutXmlInput->addWidget(mp_pushButtonSendXml);



    mp_layoutLeftSide = new QVBoxLayout;
    mp_layoutLeftSide->addWidget(mp_textEditViewXml);
    mp_layoutLeftSide->addLayout(mp_layoutXmlInput);

    mp_layoutRightSide = new QVBoxLayout;
    mp_layoutRightSide->addLayout(mp_layoutConnect);
    mp_layoutRightSide->addLayout(mp_layoutButtons);
    mp_layoutRightSide->addStretch();
    mp_layoutRightSide->setSizeConstraint(QLayout::SetFixedSize);
    //mp_layoutRightSide

    mp_layoutMain = new QHBoxLayout;
    mp_layoutMain->addLayout(mp_layoutLeftSide);
    mp_layoutMain->addLayout(mp_layoutRightSide);
    //mp_layoutMain->setStretchFactor(mp_layoutLeftSide, 30);
    //mp_layoutMain->setStretchFactor(mp_layoutRightSide, 10);

    setLayout(mp_layoutMain);
    show();

    QObject::connect(mp_pushButtonConnect, SIGNAL(clicked()),
                     this, SLOT(connectClicked()));

    QObject::connect(mp_pushButtonSendXml, SIGNAL(clicked()),
                     this, SLOT(sendClicked()));

    QObject::connect(&m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(tcpSocketError()));

    QObject::connect(&m_tcpSocket, SIGNAL(readyRead()),
                     this, SLOT(incomingData()));

    QObject::connect(&m_tcpSocket, SIGNAL(connected()),
                     this, SLOT(connected()));
    QObject::connect(&m_tcpSocket, SIGNAL(disconnected()),
                     this, SLOT(disconnected()));

    initButtons();
}

void ServerTester::connectClicked()
{
    if (m_connected)
    {
        mp_textEditViewXml->append("<font color=\"green\">Disconnecting from host.</font>");
        m_tcpSocket.disconnectFromHost();
    }
    else
    {
        QString host = mp_lineEditAddress->text();
        quint16 port = mp_lineEditPort->text().toInt();
        m_tcpSocket.connectToHost(host, port);
        mp_textEditViewXml->append("<font color=\"green\">Connecting to host.</font>");
    }
}

void ServerTester::tcpSocketError()
{

}

void ServerTester::connected()
{
    m_connected = 1;
    mp_pushButtonConnect->setText("Disconnect");
    mp_textEditViewXml->append("<font color=\"green\">Connected.</font>");
    mp_pushButtonSendXml->setEnabled(1);
}

void ServerTester::disconnected()
{
    m_connected = 0;
    mp_textEditViewXml->append("<font color=\"green\">Disconnected.</font>");
    mp_pushButtonConnect->setText("Connect");
    mp_pushButtonSendXml->setEnabled(0);
}

void ServerTester::sendClicked()
{
    Q_ASSERT(m_connected);
    mp_textEditViewXml->setTextColor(Qt::red);
    mp_textEditViewXml->append(mp_textEditInputXml->toPlainText());
    m_tcpSocket.write(mp_textEditInputXml->toPlainText().toAscii());
    mp_textEditInputXml->clear();
}

void ServerTester::incomingData()
{
    mp_textEditViewXml->setTextColor(Qt::blue);
    mp_textEditViewXml->append(m_tcpSocket.readAll());

}
