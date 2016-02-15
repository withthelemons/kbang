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
#include "common.h"
#include "tcpserver.h"
#include "gameserver.h"
#include "config.h"


/**
 * The only TcpServer constructor.
 * @param parent
 */
TcpServer::TcpServer(GameServer* parent)
 : QTcpServer(parent),
   m_hostAddress(QHostAddress(Config::instance().readString("network","server_bind_ip"))),
   m_hostAddressString(Config::instance().readString("network","server_bind_ip")),
   m_port(Config::instance().readInt("network", "server_port"))
{
    connect(this, SIGNAL(newConnection()),
            parent, SLOT(createClient()));
}


TcpServer::~TcpServer()
{
}

bool TcpServer::listen()
{
    return QTcpServer::listen(m_hostAddress, m_port);
}




