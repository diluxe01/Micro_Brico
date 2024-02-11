#ifndef DELETE_USER_H
#define DELETE_USER_H

#include <QDialog>
#include <QDialogButtonBox>
#include "../utilisateur.h"

namespace Ui {
class Delete_user;
}

class Delete_user : public QDialog
{
    Q_OBJECT

public:
    explicit Delete_user(QWidget *parent = nullptr);
    ~Delete_user();

    QDialogButtonBox* getOkButton(void);

    QString getMail() const;
    QString getUtinfo() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Delete_user *ui;
    QString mail;
    QString utinfo;
};

#endif // DELETE_USER_H
