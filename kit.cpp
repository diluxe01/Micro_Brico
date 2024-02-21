#include "kit.h"

Kit::Kit(QObject *parent)
    : QObject{parent}
{}

Kit::Kit(uint idkit, const QString &nom, const QString &description, const QDateTime &date_achat, const Money &prix_achat, const QString &texte_libre, bool en_panne, const QString &code, const Money &caution) : idkit(std::move(idkit)),
    nom(nom),
    description(description),
    date_achat(date_achat),
    prix_achat(prix_achat),
    texte_libre(texte_libre),
    en_panne(en_panne),
    code(code),
    caution(caution)
{}
