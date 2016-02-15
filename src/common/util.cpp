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

#include "util.h"
#include <cstdlib>
#include <time.h>
#include <QList>

QString randomToken(int minLength, int maxLength)
{
    Q_ASSERT(minLength <= maxLength);
    const static char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    const static int charl = strlen(chars);
    int length = minLength + ((int)qrand() % (maxLength - minLength + 1));
    char token[length+1];
    for (int i = 0; i < length; ++i)
    {
        token[i] = chars[qrand() % charl];
    }
    token[length - 1] = '\0';
    return QString(token);
}

bool randomBool(qreal probability)
{
    int random_val_size = 4096;
    int true_val_treshold = (int)(random_val_size * probability);
    int random_val = qrand() % random_val_size;
    return (random_val < true_val_treshold);
}
