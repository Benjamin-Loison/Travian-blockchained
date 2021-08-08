#ifndef DEF_RESOURCES_H
#define DEF_RESOURCES_H

#include "MyWindow.h"

void setResourcesScreen(MyWindow* window),
     addResourceCapacity(QHBoxLayout* hbox, QString name, quint32 capacity),
     addResource(QHBoxLayout* hbox, QString name, quint32 amount),
     setBackgroundColor(QWidget* qWidget, QColor color),
     addProductionLine(QVBoxLayout* qResourcesProductionVBox, QString name, quint32 production),
     addTroopLine(QVBoxLayout* qTroopsVBox, QString name, quint32 amount),
     updateResourcesAmount(),
     updateScreen(MyWindow* window);

QWidget* getResourceInfo(QString name, quint32 capacity, QColor backgroundColor, QColor foregroundColor),
       * getResource(QString name, quint32 amount);

extern qint64 timestampVillageStart,
              timestampGameRestored;

#endif
