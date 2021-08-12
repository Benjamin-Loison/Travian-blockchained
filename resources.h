#ifndef DEF_RESOURCES_H
#define DEF_RESOURCES_H

#include "MyWindow.h"

#define FARMS_NUMBER 18

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

extern quint32 initialLumberAmount,
               initialClayAmount,
               initialIronAmount,
               initialCropAmount;

extern quint8 farms[FARMS_NUMBER];
extern quint16 farmsScreen[FARMS_NUMBER][2];

#endif
