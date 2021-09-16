#ifndef DEF_RESOURCES_H
#define DEF_RESOURCES_H

#include "MyWindow.h"
#include "main.h"

#define FARMS_NUMBER 18
#define FARMS_MAX_LEVEL 20
#define CIRCLE_SIZE 25

#define LUMBER 0
#define CLAY 1
#define IRON 2
#define CROP 3
#define FREE_CROP 4
#define TIME 5
//#define CULTURE_POINTS 6
#define CULTURE_POINTS 5

#define RESOURCES_NUMBER 4
// means with free crop - and culture point too - used to be 6
#define EXTENDED_RESOURCES_NUMBER 5
//#define CONSTRUCTION_EXTENDED_RESOURCES_NUMBER 7
// means with free crop, -time- and culture point too
//#define CONSTRUCTION_EXTENDED_RESOURCES_NUMBER 7
#define CONSTRUCTION_EXTENDED_RESOURCES_NUMBER 6

#define WOODCUTTER 0
#define CLAY_PIT 1
#define IRON_MINE 2
#define CROPLAND 3

void setResourceFarmScreen(quint8 farmsIndex),
     addResourceCapacity(QHBoxLayout* hbox, QString name, quint32 capacity),
     addResource(QHBoxLayout* hbox, QString name, quint32 amount),
     setBackgroundColor(QWidget* qWidget, QColor color),
     addProductionLine(QVBoxLayout* qResourcesProductionVBox, QString name, quint32 production),
     addTroopLine(QVBoxLayout* qTroopsVBox, QString name = "none", quint32 amount = 0),
     updateResourcesAmount(quint32 currentTime = 0),
     updateScreen(/*bool refresh = false*/),
     setResourceFarmScreen(quint8 farmsIndex),
     setResourceScreen(),
     buildingGetBuilt(quint8 screenViewFarmsIndex);

QWidget* getResourceInfo(QString name, quint32 capacity, QColor backgroundColor, QColor foregroundColor),
       * getResource(QString name, quint32 amount, bool needBackground = true),
       * getImproveScreen(quint8 buildingsIndex, QString context, quint8 level, QStringList lines, QString description, QList<QPair<QString, QWidget*>> = {}, QWidget* additionnalContent = nullptr);

quint32 secondsBeforeHaving(quint8 resourcesIndex, quint32 resourceAmount);

extern quint32 timestampVillageStart,
               timestampGameClosed,
               lastActionTime,
               population,
               culturePoints;
extern QString lastAction;
extern QString resourcesNames[RESOURCES_NUMBER + 1],
               farmsNames[RESOURCES_NUMBER];

extern quint32 initialResourcesAmount[RESOURCES_NUMBER], // not mandatory here for 1D array to give array size ?
               resourcesAmount[CONSTRUCTION_EXTENDED_RESOURCES_NUMBER],
               resourcesProduction[RESOURCES_NUMBER],
               warehouseCapacity,
               granaryCapacity;

extern quint8 farms[FARMS_NUMBER],
              farmsTypes[FARMS_NUMBER];
extern quint16 farmsScreen[FARMS_NUMBER][2];

#endif
