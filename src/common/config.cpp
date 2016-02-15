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

#include <QDir>
#include "config.h"


Config* Config::smp_instance = 0;

Config::Config()
{
    QString kbangDirName;
    QString kbangConfigName = "kbang.conf";
#ifdef Q_OS_WIN
    kbangDirName = "KBang";
#else
    kbangDirName = ".kbang";
#endif
    if (QDir::home().cd(kbangDirName) == 0) {
        if (QDir::home().mkdir(kbangDirName) == 0) {
            qFatal("Cannot create KBang config directory.");
        }
    }
    QDir kbangDir = QDir::home();
    if (kbangDir.cd(kbangDirName) == 0) {
        qFatal("Cannot change to KBang config directory.");
    }
    m_configFileName = kbangDir.absolutePath() + "/" + kbangConfigName;
    if (!kbangDir.exists(kbangConfigName)) {
        createDefaultConfig();
        store();
    } else {
        refresh();
    }
}

Config::~Config()
{
    store();
}


QString Config::readString(QString group, QString varName)
{
    ConfigRecord* record = configRecord(group, varName);
    if (record == 0 || record->type != CONFIG_RECORD_SINGLE)
        return QString();
    return record->valueSingle;
}

QStringList Config::readStringList(QString group, QString varName)
{
    ConfigRecord* record = configRecord(group, varName);
    if (record == 0 || record->type != CONFIG_RECORD_LIST)
        return QStringList();
    return record->valueList;
}

int Config::readInt(QString group, QString varName) {
    ConfigRecord* record = configRecord(group, varName);
    if (record == 0 || record->type != CONFIG_RECORD_SINGLE)
        return 0;
    return record->valueSingle.toInt();
}

QList<int> Config::readIntList(QString group, QString varName)
{
    ConfigRecord* record = configRecord(group, varName);
    if (record == 0 || record->type != CONFIG_RECORD_LIST)
        return QList<int>();
    QList<int> res;
    foreach(const QString& s, record->valueList)
        res.append(s.toInt());
    return res;
}

void Config::writeString(QString group, QString varName, QString varValue)
{
    createGroupIfNeeded(group);
    m_groups[group].records[varName] = ConfigRecord(varName, CONFIG_RECORD_SINGLE, varValue);
}

void Config::writeStringList(QString group, QString varName, QStringList varValue)
{
    createGroupIfNeeded(group);
    m_groups[group].records[varName] = ConfigRecord(varName, CONFIG_RECORD_LIST, QString(), varValue);
}

void Config::writeInt(QString group, QString varName, int varValue)
{
    createGroupIfNeeded(group);
    m_groups[group].records[varName] = ConfigRecord(varName, CONFIG_RECORD_SINGLE, QString::number(varValue));
}

void Config::writeIntList(QString group, QString varName, QList<int> varValue)
{
    createGroupIfNeeded(group);
    QStringList val;
    foreach(int v, varValue)
        val.append(QString::number(v));
    m_groups[group].records[varName] = ConfigRecord(varName, CONFIG_RECORD_LIST, QString(), val);
}

bool Config::hasGroup(QString group)
{
    return m_groups.contains(group);
}

void Config::refresh()
{
    QFile file(m_configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical(qPrintable(QString("%1: cannot open config file for reading.").arg(m_configFileName)));
        return;
    }    
    QTextStream in(&file);
    ConfigGroup* currentGroup = 0;
    QString currentGroupName;
    QRegExp groupRegExp("^\\[([a-zA-Z_\\-]+)\\]$");
    QRegExp singleVarRegExp("^([a-zA-Z0-9_\\-]+)\\s*=(.*)$");
    QRegExp listVarRegExp("^([a-zA-Z0-9_\\-]+)\\[\\]\\s*=(.*)$");

    m_groups.clear();

    int lineNum = 0;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        lineNum++;
        int commentChar = line.indexOf('#');
        if (commentChar != -1) line = line.left(commentChar);
        line = line.trimmed();
        if (line.isEmpty()) continue;

        if (groupRegExp.exactMatch(line)) {

            currentGroupName = groupRegExp.capturedTexts()[1];
            createGroupIfNeeded(currentGroupName);
            currentGroup = &(m_groups[currentGroupName]);
        } else {
            if (currentGroupName.isEmpty()) {
                qWarning(qPrintable(QString("%1: %2: line is not in group.").arg(m_configFileName, lineNum)));
                continue;
            }
            if (singleVarRegExp.exactMatch(line)) {

                QString varName = singleVarRegExp.capturedTexts()[1];
                QString varValue = singleVarRegExp.capturedTexts()[2];
                if (currentGroup->records.contains(varName)) {
                    qWarning(qPrintable(QString("%1: %2: variable %3 already assigned.").arg(m_configFileName).
                                        arg(lineNum).arg(varName)));
                    continue;
                }
                currentGroup->records[varName] = ConfigRecord(varName, CONFIG_RECORD_SINGLE, varValue);
            } else if (listVarRegExp.exactMatch(line)) {
                QString varName = listVarRegExp.capturedTexts()[1];
                QString varValue = listVarRegExp.capturedTexts()[2];
                if (!currentGroup->records.contains(varName)) {
                    currentGroup->records[varName] = ConfigRecord(varName, CONFIG_RECORD_LIST, QString());
                } else if (currentGroup->records[varName].type != CONFIG_RECORD_LIST) {
                    qWarning(qPrintable(QString("%1: %2: variable %3 is not list.").arg(m_configFileName).
                                        arg(lineNum).arg(varName)));
                    continue;
                }
                currentGroup->records[varName].valueList.append(varValue);
            } else {
                qWarning(qPrintable(QString("%1: %2: cannot parse line.").arg(m_configFileName).arg(lineNum)));
            }
        }
    }
}

void Config::store()
{
    QFile file(m_configFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical(qPrintable(QString("%1: cannot open config file for writing.").arg(m_configFileName)));
        return;
    }
    QTextStream out(&file);
    foreach(const ConfigGroup& group, m_groups.values()) {
        out << "[" << group.name << "]" << endl;
        foreach(const ConfigRecord& record, group.records.values()) {
            switch(record.type) {
            case CONFIG_RECORD_SINGLE:
                out << record.name << "=" << record.valueSingle << endl;
                break;
            case CONFIG_RECORD_LIST:
                foreach(const QString& value, record.valueList) {
                    out << record.name << "[]" << "=" << value << endl;
                }
                break;
            }
        }
    }
    file.close();
}

void Config::createGroupIfNeeded(QString group)
{
    if (!m_groups.contains(group)) {
        m_groups[group] = ConfigGroup(group);
    }
}

void Config::createDefaultConfig()
{
    {
        ConfigGroup& network = m_groups["network"];
        network.name = "network";
        network.records["server_bind_ip"] = ConfigRecord("server_bind_ip", CONFIG_RECORD_SINGLE, "0.0.0.0");
        network.records["server_port"] = ConfigRecord("server_port", CONFIG_RECORD_SINGLE, "6543");
        network.records["server_name"] = ConfigRecord("server_name", CONFIG_RECORD_SINGLE, "KBang Server");
        network.records["server_description"] = ConfigRecord("server_description", CONFIG_RECORD_SINGLE, "Default Description");
    }
    {
        ConfigGroup& group = m_groups["player"];
        group.name = "player";
        group.records["name"] = ConfigRecord("name", CONFIG_RECORD_SINGLE, "Player");
        group.records["password"] = ConfigRecord("password", CONFIG_RECORD_SINGLE, "");
    }
    {
        ConfigGroup& group = m_groups["server-list"];
        group.name = "server-list";
        group.records["hostname"] = ConfigRecord("hostname", CONFIG_RECORD_LIST, QString(), QStringList() <<
                                                 "alderan.cz");
        group.records["port"] = ConfigRecord("port", CONFIG_RECORD_LIST, QString(), QStringList() <<
                                                 "6543");
    }
    {
        ConfigGroup& group = m_groups["server"];
        group.name = "server";
        group.records["wipe-ai-only-game"] = ConfigRecord("wipe-ai-only-game", CONFIG_RECORD_SINGLE, "true");
    }
}

Config::ConfigRecord* Config::configRecord(QString group, QString varName) {
    if (!m_groups.contains(group) || !m_groups[group].records.contains(varName))
        return 0;
    return &(m_groups[group].records[varName]);
}


/* static */ QString
Config::dataPathString()
{
    QString path(QCoreApplication::instance()->applicationDirPath());

    #ifdef Q_OS_WIN32
        path += "/data/";
    #else
        #ifdef Q_WS_MAC
            if (QRegExp("Contents/MacOS/?$").indexIn(path) != -1) {
                // pointing into an macosx application bundle
                path += "/../Resources/data/";
            } else {
                path += "/data/";
            }
        #else //Unix
            if (QRegExp("kbang/?$").indexIn(path) != -1) {
                // there is an own application directory
                path += "/data/";
            } else if (QRegExp("usr/games/bin/?$").indexIn(path) != -1) {
                // we are in /usr/games/bin (like gentoo linux does)
                path += "/../../share/games/kbang/data/";
            } else if (QRegExp("usr/games/?$").indexIn(path) != -1) {
                // we are in /usr/games (like Debian linux does)
                path += "/../share/games/kbang/";
            } else if (QRegExp("bin/?$").indexIn(path) != -1) {
                // we are in a bin directory. e.g. /usr/bin
                path += "/../share/kbang/data/";
            } else {
                path += "/data/";
            }
        #endif
    #endif
    return (QDir::cleanPath(path) + "/");
}
