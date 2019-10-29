#include "qlistwidgetextended.h"

QListWidgetExtended::QListWidgetExtended():QListWidget()
{

}

QListWidgetExtended::QListWidgetExtended(QWidget* & wid):QListWidget(wid)
{
}

void QListWidgetExtended::currentPositionClickedHandler(QVector<QString> data)
{
    this->clear();
    for(auto i:data)
    {
       this->addItem(i);
    }
}
