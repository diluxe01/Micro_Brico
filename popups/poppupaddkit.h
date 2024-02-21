#ifndef POPPUPADDKIT_H
#define POPPUPADDKIT_H

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class PoppupAddKit;
}

class PoppupAddKit : public QDialog
{
    Q_OBJECT

public:
    explicit PoppupAddKit(QWidget *parent = nullptr);
    QDialogButtonBox* getOkButton(void);
    ~PoppupAddKit();

private slots:
    void on_pushButton_addobject_clicked();

private:
    Ui::PoppupAddKit *ui;
};

#endif // POPPUPADDKIT_H
