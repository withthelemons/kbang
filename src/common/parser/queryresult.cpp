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

#include "queryresult.h"
#include "parser.h"
#include <QXmlStreamWriter>

QueryResult::QueryResult(QXmlStreamWriter* streamWriter, const QString& id):
mp_streamWriter(streamWriter), m_id(id), m_sent(0)
{
}

void QueryResult::sendPong()
{
    writeStartQuery();
    mp_streamWriter->writeEmptyElement("pong");
    writeEndQuery();
}

void QueryResult::writeStartQuery()
{
    mp_streamWriter->writeStartElement("query");
    mp_streamWriter->writeAttribute("type", "result");
    mp_streamWriter->writeAttribute("id", m_id);
}

void QueryResult::writeEndQuery()
{
    mp_streamWriter->writeEndElement();
}
