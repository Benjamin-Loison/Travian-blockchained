#include "troops.h"
#include "resources.h"

/*quint32 troopsTimes[TRIBES_NUMBER][TROOPS_NUMBER] = {
    {},
    {},
    {}
};*/

// lumber, clay, iron, crop, time
quint32 researchCosts[TRIBES_NUMBER][TROOPS_NUMBER][CONSTRUCTION_EXTENDED_RESOURCES_NUMBER] = {
    {{0, 0, 0, 0, 0}, {940, 700, 1680, 520, 6120}, {1120, 700, 360, 400, 5880}, {2200, 1900, 2040, 520, 9240}, {2260, 1420, 2440, 880, 9480}, {3100, 2580, 5600, 1180, 11160}, {5800, 2320, 2840, 610, 16800}, {5860, 5900, 5240, 700, 28800}, {15880, 22900, 25200, 22660, 24475}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {700, 620, 1480, 580, 7080}, {1000, 740, 1880, 640, 7560}, {940, 740, 360, 400, 5880}, {3400, 1860, 2760, 760, 9720}, {3400, 2660, 6600, 1240, 12360}, {5500, 1540, 4200, 580, 15600}, {5800, 5500, 5000, 700, 28800}, {15880, 13800, 36400, 22660, 24475}, {0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}, {970, 380, 880, 400, 5160}, {880, 580, 1560, 580, 5400}, {1060, 500, 600, 460, 5160}, {2320, 1180, 2520, 610, 9000}, {2800, 2160, 4040, 640, 10680}, {6100, 1300, 3000, 580, 14400}, {5500, 4900, 5000, 520, 28800}, {18250, 13500, 20400, 16480, 19425}, {0, 0, 0, 0, 0}}
};

// lumber, clay, iron, crop, cropfree, time, attack, defense infantry, defense cavalry, speed, carry
/*quint16*/quint32 troopsCosts[TRIBES_NUMBER][TROOPS_NUMBER][11] = {
    {{100, 130, 55, 30, 1, 1040, 15, 40, 50, 7, 35}, {140, 150, 185, 60, 1, 1440, 65, 35, 20, 6, 45}, {170, 150, 20, 40, 2, 1360, 0, 20, 10, 17, 0}, {350, 450, 230, 60, 2, 2480, 90, 25, 40, 19, 75}, {360, 330, 280, 120, 2, 2560, 45, 115, 55, 16, 35}, {500, 620, 675, 170, 3, 3120, 140, 60, 165, 13, 65}, {950, 555, 330, 75, 3, 5000, 50, 30, 105, 4, 0}, {960, 1450, 630, 90, 6, 9000, 70, 45, 10, 3, 0}, {30750, 45400, 31000, 37500, 4, 90700, 40, 50, 50, 5, 0}, {4400, 5600, 4200, 3900, 1, 22700, 0, 80, 80, 5, 3000}},
    {{120, 100, 150, 30, 1, 1600, 40, 35, 50, 6, 50}, {100, 130, 160, 70, 1, 1760, 30, 65, 35, 5, 20}, {150, 160, 210, 80, 1, 1920, 70, 40, 25, 7, 50}, {140, 160, 20, 40, 2, 1360, 0, 20, 10, 16, 0}, {550, 440, 320, 100, 3, 2640, 120, 65, 50, 14, 100}, {550, 640, 800, 180, 4, 3520, 180, 80, 105, 10, 70}, {900, 360, 500, 70, 3, 4600, 60, 30, 75, 4, 0}, {950, 1350, 600, 90, 6, 9000, 75, 60, 10, 3, 0}, {30750, 27200, 45000, 37500, 5, 90700, 50, 40, 30, 4, 0}, {4600, 4200, 5800, 4400, 1, 26900, 0, 80, 80, 5, 3000}},
    {{95, 75, 40, 40, 1, 720, 40, 20, 5, 7, 60}, {145, 70, 85, 40, 1, 1120, 10, 35, 60, 7, 40}, {130, 120, 170, 70, 1, 1200, 60, 30, 30, 6, 50}, {160, 100, 50, 50, 1, 1120, 0, 10, 5, 9, 0}, {370, 270, 290, 75, 2, 2400, 55, 100, 40, 10, 110}, {450, 515, 480, 80, 3, 2960, 150, 50, 75, 9, 80}, {1000, 300, 350, 70, 3, 4200, 65, 30, 80, 4, 0}, {900, 1200, 600, 60, 6, 9000, 50, 60, 10, 3, 0}, {35500, 26600, 25000, 27200, 4, 70500, 40, 60, 40, 4, 0}, {5800, 4400, 4600, 5200, 1, 31000, 10, 80, 80, 5, 3000}}
};

QString troopsNames[TRIBES_NUMBER][TROOPS_NUMBER] = {
    {QT_TRANSLATE_NOOP("troops", "phalanx"), QT_TRANSLATE_NOOP("troops", "swordsman"), QT_TRANSLATE_NOOP("troops", "pathfinder"), QT_TRANSLATE_NOOP("troops", "theutates thunder"), QT_TRANSLATE_NOOP("troops", "druidrider"), QT_TRANSLATE_NOOP("troops", "haeduan"), QT_TRANSLATE_NOOP("troops", "ram"), QT_TRANSLATE_NOOP("troops", "trebuchet"), QT_TRANSLATE_NOOP("troops", "chieftain"), QT_TRANSLATE_NOOP("troops", "settler")},
    {QT_TRANSLATE_NOOP("troops", "legionnaire"), QT_TRANSLATE_NOOP("troops", "praetorian"), QT_TRANSLATE_NOOP("troops", "imperian"), QT_TRANSLATE_NOOP("troops", "equites legati"), QT_TRANSLATE_NOOP("troops", "equites imperatoris"), QT_TRANSLATE_NOOP("troops", "equites caesaris"), QT_TRANSLATE_NOOP("troops", "battering ram"), QT_TRANSLATE_NOOP("troops", "fire catapult"), QT_TRANSLATE_NOOP("troops", "senator"), QT_TRANSLATE_NOOP("troops", "settler")},
    {QT_TRANSLATE_NOOP("troops", "clubswinger"), QT_TRANSLATE_NOOP("troops", "spearman"), QT_TRANSLATE_NOOP("troops", "axeman"), QT_TRANSLATE_NOOP("troops", "scout"), QT_TRANSLATE_NOOP("troops", "paladin"), QT_TRANSLATE_NOOP("troops", "teutonic knight"), QT_TRANSLATE_NOOP("troops", "ram"), QT_TRANSLATE_NOOP("troops", "catapult"), QT_TRANSLATE_NOOP("troops", "chief"), QT_TRANSLATE_NOOP("troops", "settler")}
};

quint32 troops[TROOPS_NUMBER] = {
    0, 1, 2, 0, 0, 0, 0, 0, 0, 0
};

quint8 troopsTypes[TRIBES_NUMBER][TROOPS_NUMBER] = {
    {INFANTRY, INFANTRY, CAVALRY, CAVALRY, CAVALRY, CAVALRY, SIEGE, SIEGE, CHIEF, SETTLER},
    {INFANTRY, INFANTRY, INFANTRY, CAVALRY, CAVALRY, CAVALRY, SIEGE, SIEGE, CHIEF, SETTLER},
    {INFANTRY, INFANTRY, INFANTRY, INFANTRY, CAVALRY, CAVALRY, SIEGE, SIEGE, CHIEF, SETTLER},
};

QString getTroopName(quint8 troopsIndex)
{
    return troopsNames[tribe][troopsIndex];
}
