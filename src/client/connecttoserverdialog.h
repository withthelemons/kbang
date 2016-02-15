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
#ifndef CONNECTTOSERVERDIALOG_H
#define CONNECTTOSERVERDIALOG_H

#include <QDialog>
#include <ui_connecttoserverdialog.h>

namespace client
{

/**
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class ConnectToServerDialog : public QDialog, public Ui::ConnectToServerDialog
{
Q_OBJECT
public:
    ConnectToServerDialog(QWidget *parent = 0);
    ~ConnectToServerDialog();

    void reloadServerList();

private slots:
    void on_serverList_currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
    void on_serverList_itemDoubleClicked (QTreeWidgetItem*, int);
    void on_pushButtonAddServer_clicked();
    void on_pushButtonEditServer_clicked();
    void on_pushButtonDeleteServer_clicked();
    void on_pushButtonConnect_clicked();

signals:
    void connectToServer(QString serverHost, int serverPort);

private:
    void loadConfigValues();
    void saveConfigValues();
};
}
#endif
