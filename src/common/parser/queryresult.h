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

#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include "parserstructs.h"
#include <QString>

class QXmlStreamWriter;


class QueryResult
{
public:
    QueryResult(QXmlStreamWriter* streamWriter, const QString& id);

    template<class T>
    void sendData(const T& data)
    {
        writeStartQuery();
        data.write(mp_streamWriter);
        writeEndQuery();
    }

    void sendPong();

private:
    void writeStartQuery();
    void writeEndQuery();

    QXmlStreamWriter* mp_streamWriter;
    QString m_id;
    bool    m_sent;
};


#endif
