#ifndef DEF_MAIN_H
#define DEF_MAIN_H

#include <QTranslator>
#include "MyWindow.h"
#include "troops.h"

#define BUILDINGS_LEVELS_NUMBER 20
#define FARMS_LEVELS_NUMBER 21
// here otherwise circular build

#define SEPARATOR ","

#define getSeconds() QDateTime::currentSecsSinceEpoch()
//#define getSeconds() 172220
#define getDateTime() QDateTime::fromSecsSinceEpoch(getSeconds())
// likewise we can manually modify current time for testing

extern QString USER_FOLDER;

enum screenViewEnum{SCREEN_VIEW_SELECT_TRIBE,
                    SCREEN_VIEW_SELECT_LOCATION,
                    SCREEN_VIEW_RESOURCES,
                    SCREEN_VIEW_RESOURCE,
                    SCREEN_VIEW_BUILDINGS,
                    SCREEN_VIEW_BUILDING,
                    SCREEN_VIEW_STATISTICS,
                    SCREEN_VIEW_MAP,
                    SCREEN_VIEW_REPORTS,
                    SCREEN_VIEW_PROFILE,
                    SCREEN_VIEW_VILLAGE};

extern screenViewEnum screenView,
                      lastScreenView;
extern tribeEnum tribe;
extern QTranslator translator;
extern MyWindow* myWindow;
extern QString nickname;
extern quint8 screenViewFarmsIndex,
              screenViewBuildingsIndex;
extern QPair<quint16, quint16> coordinates;

QString getTribe(),
        getScreenView(),
        getTribe(tribeEnum tribe),
        getCoordinatesStr();

#endif
