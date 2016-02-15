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
#include "connecttoserverdialog.h"
#include "newserverdialog.h"
#include "config.h"

using namespace client;

ConnectToServerDialog::ConnectToServerDialog(QWidget *parent)
 : QDialog(parent)
{
    setupUi(this);
    loadConfigValues();
}


ConnectToServerDialog::~ConnectToServerDialog()
{
}

void ConnectToServerDialog::reloadServerList()
{
}

void ConnectToServerDialog::on_serverList_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    Q_UNUSED(previous);
    pushButtonConnect->setEnabled(current != 0);
}

void ConnectToServerDialog::on_serverList_itemDoubleClicked (QTreeWidgetItem*, int)
{
    on_pushButtonConnect_clicked();
}

void ConnectToServerDialog::on_pushButtonAddServer_clicked()
{
    NewServerDialog dialog(this);
    dialog.setupForNewServer();
    if (!dialog.exec())
        return;
    QStringList strings;
    strings << dialog.host() << QString::number(dialog.port());
    serverList->addTopLevelItem(new QTreeWidgetItem(serverList, strings));
    saveConfigValues();
}

void ConnectToServerDialog::on_pushButtonEditServer_clicked()
{
    QTreeWidgetItem* item = serverList->currentItem();
    Q_ASSERT(item != 0);

    NewServerDialog dialog(this);
    dialog.setupForEditServer(item->text(0), item->text(1).toInt());
    if (!dialog.exec())
        return;

    item->setText(0, dialog.host());
    item->setText(1, QString::number(dialog.port()));
    saveConfigValues();
}

void ConnectToServerDialog::on_pushButtonDeleteServer_clicked()
{
    QTreeWidgetItem* item = serverList->currentItem();
    Q_ASSERT(item != 0);
    delete item;
}

void ConnectToServerDialog::on_pushButtonConnect_clicked()
{
    Q_ASSERT(serverList->currentItem() != 0);
    QString host = serverList->currentItem()->text(0);
    int port = serverList->currentItem()->text(1).toInt();
    emit connectToServer(host, port);
    close();
}

void ConnectToServerDialog::loadConfigValues()
{
    Config& cfg = Config::instance();
    cfg.refresh();
    QStringList hosts = cfg.readStringList("server-list", "hostname");
    QList<int>  ports = cfg.readIntList("server-list", "port");
    int n = hosts.size() < ports.size() ? hosts.size() : ports.size();
    serverList->clear();
    for (int i = 0; i < n; ++i) {
        QStringList strings;
        strings << hosts[i] << QString::number(ports[i]);
        serverList->addTopLevelItem(new QTreeWidgetItem(serverList, strings));
    }
}

void ConnectToServerDialog::saveConfigValues()
{
    Config& cfg = Config::instance();
    cfg.refresh();
    QStringList hosts;
    QList<int> ports;
    for(int i = 0; i < serverList->topLevelItemCount(); ++i) {
        hosts.append(serverList->topLevelItem(i)->text(0));
        ports.append(serverList->topLevelItem(i)->text(1).toInt());
    }
    cfg.writeStringList("server-list", "hostname", hosts);
    cfg.writeIntList("server-list", "port", ports);
    cfg.store();
}
