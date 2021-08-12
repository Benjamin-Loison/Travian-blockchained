#ifndef DEF_MAIN_H
#define DEF_MAIN_H

#include <QTranslator>
#include "MyWindow.h"

enum screenViewEnum{SCREEN_VIEW_SELECT_TRIBE, SCREEN_VIEW_SELECT_LOCATION, SCREEN_VIEW_RESOURCES, SCREEN_VIEW_BUILDINGS, SCREEN_VIEW_STATISTICS};
enum tribeEnum{TRIBE_GAULS, TRIBE_ROMANS, TRIBE_TEUTONS};

extern screenViewEnum screenView;
extern tribeEnum tribe;
extern QTranslator translator;
extern MyWindow* window;
extern QString nickname;

QString getTribe();

#endif
