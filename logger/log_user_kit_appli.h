#ifndef LOG_USER_KIT_APPLI_H
#define LOG_USER_KIT_APPLI_H

#include <QWidget>
#include "../connect_db.h"
#include "../datamodel.h"
#include "../kit.h"
#include "../utilisateur.h"

namespace Ui {
class log_user_kit_appli;
}

class log_user_kit_appli : public QWidget
{
    Q_OBJECT

public:
    explicit log_user_kit_appli(QWidget *parent = nullptr);
    ~log_user_kit_appli();

    void setKit(Kit *newKit);

    void setUser(Utilisateur *newUser);
    void update_logs_from_user(int i_max_lines);
    void update_logs_from_kit(int i_max_lines);

private:
    Ui::log_user_kit_appli *ui;
    std::vector<Log*> log_list;
    Kit* kit;
    Utilisateur* user;

};

#endif // LOG_USER_KIT_APPLI_H
