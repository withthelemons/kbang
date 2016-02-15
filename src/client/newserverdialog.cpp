#include "newserverdialog.h"

using namespace client;

NewServerDialog::NewServerDialog(QWidget *parent):
        QDialog(parent)
{
    setupUi(this);
    spinBoxPort->setMinimum(1);
    spinBoxPort->setMaximum(65535);
    spinBoxPort->setValue(6543);
}

void NewServerDialog::setupForNewServer()
{
    setWindowTitle(tr("Add Server"));
}

void NewServerDialog::setupForEditServer(const QString& host, int port)
{
    setWindowTitle(tr("Edit Server"));
    lineEditHost->setText(host);
    spinBoxPort->setValue(port);
}

QString NewServerDialog::host()
{
    return lineEditHost->text();
}

int NewServerDialog::port()
{
    return spinBoxPort->value();
}
