#ifndef TYPES_H
#define TYPES_H

enum T_modify_or_new
{
    E_modify,
    E_new
};

enum T_user_privilege
{
    E_admin,
    E_basic
};

typedef enum T_SORTIE_RESTIT
{
    E_MODE_SORTIE,
    E_MODE_RESTITUTION
}T_SORTIE_RESTIT;

typedef enum T_ITEM_STATE
{
    E_STATE_UNCHANGED,
    E_STATE_EDITED,
    E_STATE_ADDED,
    E_STATE_DELETED,
}T_ITEM_STATE;

#endif // TYPES_H
