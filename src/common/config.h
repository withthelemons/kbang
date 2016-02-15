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
#ifndef CONFIG_H
#define CONFIG_H

#include "util.h"

#include <QtCore>

/**
 * The config class provides a way to handle configuration files and commandline
 * arguments. This class is a singleton.
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
*/
class Config: private NonCopyable {

public:
    QString         readString(QString group, QString varName);
    QStringList  readStringList(QString group, QString varName);
    int             readInt(QString group, QString varName);
    QList<int>      readIntList(QString group, QString varName);

    void writeString(QString group, QString varName, QString varValue);
    void writeStringList(QString group, QString varName, QStringList varValue);
    void writeInt(QString group, QString varName, int varValue);
    void writeIntList(QString group, QString varName, QList<int> varValue);

    bool hasGroup(QString group);

    void refresh();
    void store();

    static inline Config& instance() {
        if (!smp_instance)
            smp_instance = new Config();
        return *smp_instance;
    }

    static QString dataPathString();

private:
    Config();
    ~Config();

    enum ConfigRecordType {
        CONFIG_RECORD_SINGLE,
        CONFIG_RECORD_LIST
    };

    struct ConfigRecord {
        ConfigRecord() {}
        ConfigRecord(QString n, ConfigRecordType t, QString vs, QStringList vl = QStringList()):
                name(n), type(t), valueSingle(vs), valueList(vl) {}
        QString name;
        ConfigRecordType type;
        QString valueSingle;
        QStringList valueList;
    };

    struct ConfigGroup {
        ConfigGroup() {}
        ConfigGroup(QString n): name(n) {}
        QString name;
        QMap<QString, ConfigRecord> records;
    };

    void createGroupIfNeeded(QString group);
    void createDefaultConfig();
    ConfigRecord* configRecord(QString group, QString varName);

    QString m_configFileName;
    QMap<QString, ConfigGroup> m_groups;
    static Config* smp_instance;

};

#endif
