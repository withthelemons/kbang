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
#ifndef XMLNODE_H
#define XMLNODE_H

#include <QObject>
#include <QPair>
#include <QString>
#include <QMap>
#include <QXmlStreamAttributes>

class QStringRef;

/**
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class XmlNode : public QObject
{
Q_OBJECT
public:
    XmlNode(XmlNode *parent, const QString& elementName, bool textElement = 0);
    XmlNode* createChildNode(const QString& elementName);
    XmlNode* createChildNode(const QStringRef& elementName, const QXmlStreamAttributes& attributes);
    XmlNode* createChildTextNode(const QStringRef& text);
    void     createAttribute(const QString& name, const QString& value);
    void     createAttributes(const QXmlStreamAttributes& attributes);
    
    void     debugPrint(int indent = 0) const;
    
    QString name() const;
    QString attribute(const QString& name) const;
    QString text() const;
    bool isTextElement() const;
    XmlNode* getFirstChild() const;
    QList<XmlNode*> getChildren() const;
    
    
    virtual XmlNode* parentNode();
    virtual ~XmlNode();

private:
    const QString           m_elementName;
    bool                    m_isTextElement;
    QMap<QString, QString>  m_attributes;
    QList<XmlNode*>         m_children;
};

#endif
