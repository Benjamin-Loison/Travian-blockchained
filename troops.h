#ifndef DEF_TROOPS_H
#define DEF_TROOPS_H

#include <QString>
//#include "resources.h"
//#include "main.h"

#define TRIBES_NUMBER 3

enum tribeEnum{TRIBE_GAULS,
               TRIBE_ROMANS,
               TRIBE_TEUTONS};
// to avoid circular build

#define TROOPS_NUMBER 10
// could use enum but making a for on an enum values I don't like that
#define INFANTRY 0
#define CAVALRY 1
#define SIEGE 2
#define CHIEF 3
#define SETTLER 4

extern QString troopsNames[TRIBES_NUMBER][TROOPS_NUMBER];

extern quint32 troops[TROOPS_NUMBER];
extern quint32 troopsCosts[TRIBES_NUMBER][TROOPS_NUMBER][11];
extern quint32 researchCosts[TRIBES_NUMBER][TROOPS_NUMBER][6/*CONSTRUCTION_EXTENDED_RESOURCES_NUMBER*/];

QString getTroopName(quint8 troopsIndex);

#endif
