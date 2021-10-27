#ifndef DEF_BUILDINGS_H
#define DEF_BUILDINGS_H

//#include <QtGlobal>
#include <QWidget>
#include "resources.h"
#include "main.h"

#define BUILDINGS_NUMBER 34
#define BUILDINGS_SLOTS_NUMBER 22
#define BUILDING_SLOT_WALL 21
// building level 0 doesn't have associated characteristics except for warehouse and granary for instance in comparaison with farms

#define MAIN_BUILDING 0
#define RALLY_POINT 1
#define CRANNY 2
#define WAREHOUSE 3
#define GRANARY 4
#define EMBASSY 5
#define MARKETPLACE 6
#define RESIDENCE 7
#define PALACE 8
#define STONEMASONS_LODGE 9
#define TREASURY 10
#define TOWN_HALL 11
#define TRADE_OFFICE 12
#define GREAT_GRANARY 13
#define GREAT_WAREHOUSE 14
#define BARRACKS 15
#define HEROS_MANSION 16
#define ACADEMY 17
#define SMITHY 18
#define STABLE 19
#define WORKSHOP 20
#define TOURNAMENT_SQUARE 21
#define GRAIN_MILL 22
#define SAWMILL 23
#define BRICKYARD 24
#define IRON_FOUNDRY 25
#define BAKERY 26
#define TRAPPER 27
#define HORSE_DRINKING_TROUGH 28
#define BREWERY 29
#define PALISADE 30
#define CITY_WALL 31
#define EARTH_WALL 32
#define WONDER_OF_THE_WORLD 33
// if really want to have spare time could not do: palace, trapper, horse drinking trough, brewery

void setBuildingsScreen(),
     setBuildingScreen(quint8 buildingsIndex),
     constructBuilding();
extern quint16 buildingsScreen[BUILDINGS_SLOTS_NUMBER][2];
QWidget* getBuildingScreen(),
       * getBuildingImprovementScreen();
extern QPair<quint8, quint8> buildings[BUILDINGS_SLOTS_NUMBER];
extern QString buildingsNames[BUILDINGS_NUMBER];
bool isAWall(quint8 buildingType);
extern quint8 buildingsLevelsNumber[BUILDINGS_NUMBER];
extern quint32 buildingsCosts[RESOURCES_NUMBER + BUILDINGS_NUMBER][/*FARMS_LEVELS_NUMBER*/101][CONSTRUCTION_EXTENDED_RESOURCES_NUMBER];
quint32 getTimeForBuilding(quint8 buildingId, quint8 level);

#endif
