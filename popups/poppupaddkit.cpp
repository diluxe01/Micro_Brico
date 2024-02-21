#include "poppupaddkit.h"
#include "ui_poppupaddkit.h"

PoppupAddKit::PoppupAddKit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PoppupAddKit)
{
    ui->setupUi(this);
}

PoppupAddKit::~PoppupAddKit()
{
    delete ui;
}

QDialogButtonBox* PoppupAddKit::getOkButton(void)
{
    return ui->buttonBox;
}

void PoppupAddKit::on_pushButton_addobject_clicked()
{

}

