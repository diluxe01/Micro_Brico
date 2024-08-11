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
    //Set user email inside popup label
    this->ui->label_set_user_name->setText(this->user->getEmail());
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

Kit *PopupSortirResa::getP_kit() const
{
    return p_kit;
}

void PopupSortirResa::setP_kit(Kit *newP_kit)
{
    p_kit = newP_kit;
}


///
/// \brief PopupSortirResa::get_sel_item_from_QlistWidget
/// \param i_QlistWidget: QlistWidget where user has selected an item
/// \param i_item_list: list of actual items
/// \return item in the list corresponding to selected item in QlistWidget
///
Item* PopupSortirResa::get_sel_item_from_QlistWidget(QListWidget * i_QlistWidget, std::vector<Item*> i_item_list )
{
    int row = i_QlistWidget->currentRow();
    Item* p_item = i_item_list.at(row);
    return p_item;
}


Item * PopupSortirResa::get_item_from_id( std::vector<Item*> i_item_list,uint i_id  )
{
    Item * return_item = NULL;
    for (const auto& elem_item : i_item_list)
    {
        if (elem_item->getId() == i_id)
        {
            return_item = elem_item;
        }
    }
    return return_item;
}



void PopupSortirResa::push_item_to_dest_list(int new_quantity)
{
    Item* p_itemsource = get_sel_item_from_QlistWidget(this->ui->listWidget_popupSortie_ItemSource, p_kit->item_list);

    // Create new item with new quantity (even if it is unchanged)
    Item* p_item = new Item(p_itemsource->getId(), p_itemsource->getName() , 0, new_quantity);

    // push this new item to local list
    this->item_list_dest.push_back(p_item);

    //Remember that item has been verified in source list
    p_itemsource->setIs_verified(true);

    refresh_source_item_list();
    refresh_dest_item_list();
}


void PopupSortirResa::refresh_source_item_list(void)
{
    QBrush brush_verified;
    // Define brush to display kit booked in kit reservation list
    brush_verified.setColor(Qt::GlobalColor::gray);
    brush_verified.setStyle(Qt::SolidPattern);

    this->ui->listWidget_popupSortie_ItemSource->clear();

    for(const auto& elem_item_new : p_kit->item_list)
    {
        QListWidgetItem* p_item = new QListWidgetItem(elem_item_new->getName()+" ("+QString::number(elem_item_new->getQuantity())+")", this->ui->listWidget_popupSortie_ItemSource);
        if(elem_item_new->getIs_verified())
        {
            p_item->setBackground(brush_verified);
        }
        else
        {
            // do nothing
        }
    }
}

void PopupSortirResa::refresh_dest_item_list(void)
{

    this->ui->listWidget_popupSortie_ItemDest->clear();
    for(const auto& elem_item_new : this->item_list_dest)
    {
        new QListWidgetItem(elem_item_new->getName()+" ("+QString::number(elem_item_new->getQuantity())+")", this->ui->listWidget_popupSortie_ItemDest);
    }
}

void PopupSortirResa::on_listWidget_popupSortie_ItemSource_itemDoubleClicked(QListWidgetItem *item)
{

    Item* p_itemsource = get_sel_item_from_QlistWidget(this->ui->listWidget_popupSortie_ItemSource, p_kit->item_list);

    if (p_itemsource->getIs_verified())
    {
        //Do nothing if item is already verified
    }
    else
    {
        int value = this->ui->spinBox_popupSortie_countItemVerif->value();
        push_item_to_dest_list(value);
    }
}

void PopupSortirResa::on_listWidget_popupSortie_ItemDest_itemDoubleClicked(QListWidgetItem *item)
{

    // get selected item from dest list
    Item* p_itemdest = get_sel_item_from_QlistWidget(this->ui->listWidget_popupSortie_ItemDest, this->item_list_dest);

    // get associated item from source list
    Item* p_itemsource = get_item_from_id(p_kit->item_list, p_itemdest->getId());
    // Set source item in "unverified" state
    p_itemsource->setIs_verified(false);


    //get selected item from Dest Qlistwidget and delete it
    QListWidgetItem * p_widgetItem = this->ui->listWidget_popupSortie_ItemDest->currentItem();
    this->ui->listWidget_popupSortie_ItemDest->removeItemWidget(p_widgetItem);


    // remove item from dest list
    auto it = std::find(this->item_list_dest.begin(), this->item_list_dest.end(),
                        p_itemdest);

    if (it != this->item_list_dest.end()) {
        this->item_list_dest.erase(it);
    }

    //Refresh everybody
    refresh_source_item_list();
    refresh_dest_item_list();
}


///
/// \brief PopupSortirResa::on_listWidget_popupSortie_ItemSource_itemClicked
/// Set quantity automatically when item is clicked
/// \param item
///
void PopupSortirResa::on_listWidget_popupSortie_ItemSource_itemClicked(QListWidgetItem *item)
{
    // get selected item from source list
    Item* p_itemsource = get_sel_item_from_QlistWidget(this->ui->listWidget_popupSortie_ItemSource, p_kit->item_list);


    this->ui->spinBox_popupSortie_countItemVerif->setValue(p_itemsource->getQuantity());
}

void PopupSortirResa::on_pushButton_popupSortie_pushDest_clicked()
{
    Item* p_itemsource = get_sel_item_from_QlistWidget(this->ui->listWidget_popupSortie_ItemSource, p_kit->item_list);

    if (p_itemsource->getIs_verified())
    {
        //Do nothing if item is already verified
    }
    else
    {
        int value = this->ui->spinBox_popupSortie_countItemVerif->value();
        push_item_to_dest_list(value);
    }
}

