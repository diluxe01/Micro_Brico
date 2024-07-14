#ifndef POPUPSORTIRRESA_H
#define POPUPSORTIRRESA_H

#include <QWidget>

#include <QPushButton>
#include "../utilisateur.h"

namespace Ui {
class PopupSortirResa;
}

class PopupSortirResa : public QWidget
{
    Q_OBJECT

public:
    explicit PopupSortirResa(QWidget *parent = nullptr);
    ~PopupSortirResa();

    QPushButton * getSortirButton();
    void setUser (Utilisateur *newUser);
    bool checkIfOk (void);

private:
    Ui::PopupSortirResa *ui;
    Utilisateur *user;
    void GEN_raise_popup_warning(QString msg);
};

#endif // POPUPSORTIRRESA_H
