#ifndef NEWSERVERDIALOG_H
#define NEWSERVERDIALOG_H

#include <QDialog>
#include <ui_newserverdialog.h>

namespace client {

class NewServerDialog: public QDialog, public Ui::NewServerDialog
{
Q_OBJECT
public:
    NewServerDialog(QWidget *parent);

    void setupForNewServer();
    void setupForEditServer(const QString& host, int port);

    QString host();
    int port();
};

}

#endif // NEWSERVERDIALOG_H
