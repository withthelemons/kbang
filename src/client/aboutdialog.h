#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
class AboutDialog;
}

namespace client {

class AboutDialog : public QDialog {
Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);
    virtual ~AboutDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AboutDialog *mp_ui;
};

}

#endif // ABOUTDIALOG_H
