#include "popupsortirresa.h"
#include "ui_popupsortirresa.h"
#include <QMessageBox>
PopupSortirResa::PopupSortirResa(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PopupSortirResa)
{
    ui->setupUi(this);
}

PopupSortirResa::~PopupSortirResa()
{
    delete ui;
}


void PopupSortirResa::GEN_raise_popup_warning(QString msg)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(msg);
    msgBox.setTextFormat(Qt::TextFormat::MarkdownText);
    msgBox.setWindowTitle("Attention");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    qDebug() << msg;
}

QPushButton * PopupSortirResa::getSortirButton(void)
{
    return ui->pushButton_sortir;
}

void PopupSortirResa::setUser(Utilisateur *newUser)
{
    user = newUser;
}

bool PopupSortirResa::checkIfOk()
{
    bool returnValue = true;
    if (this->ui->lineEdit_mdp->text()!= this->user->getMdp())
    {
        GEN_raise_popup_warning("Erreur dans le mot de passe de l'utilisateur.");
        returnValue = false;
    }
    return returnValue;
}
