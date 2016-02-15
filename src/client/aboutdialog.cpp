#include <QApplication>
#include "parser/parser.h"
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

using namespace client;

AboutDialog::AboutDialog(QWidget *parent):
        QDialog(parent),
        mp_ui(new Ui::AboutDialog)
{
    mp_ui->setupUi(this);
    mp_ui->labelAppName->setText(QApplication::instance()->applicationName());
    mp_ui->labelAppVersion->setText(QString("Version: %1").arg(
            QApplication::instance()->applicationVersion()));
    mp_ui->labelProtocolVersion->setText(QString("Protocol version: %1").arg(Parser::protocolVersion()));
}

AboutDialog::~AboutDialog()
{
    delete mp_ui;
}

void AboutDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        mp_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
