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

#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QList>
#include <QLinkedList>
#include <cstdlib>
#include <qglobal.h>


#define NOT_REACHED() qFatal("Fatal Error: NOT_REACHED triggered at line %d of %s", __LINE__, __FILE__)
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

QString randomToken(int minLength, int maxLength);
bool randomBool(qreal probability);

template <typename T>
inline void shuffleList(QList<T>& list)
{
    int size = list.count();
    int swapCount = size * 4;
    while(swapCount-- != 0)
    {
        list.swap(qrand() % size, rand() % size);
    }
}

template <typename T>
inline void shuffleList(QLinkedList<T>& list)
{
    int size = list.count();
    int swapCount = size * 4;
    while(swapCount-- != 0)
    {
        list.swap(qrand() % size, rand() % size);
    }
}

class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};


#endif
