#ifndef DEF_RESOURCES_H
#define DEF_RESOURCES_H

#include "MyWindow.h"

void setResourcesScreen(MyWindow* window),
     addResourceCapacity(QHBoxLayout* hbox, QString name, unsigned int capacity),
     addResource(QHBoxLayout* hbox, QString name, unsigned int amount),
     setBackgroundColor(QWidget* qWidget, QColor color),
     addProductionLine(QVBoxLayout* qResourcesProductionVBox, QString name, unsigned int production),
     addTroopLine(QVBoxLayout* qTroopsVBox, QString name, unsigned int amount);

#endif
