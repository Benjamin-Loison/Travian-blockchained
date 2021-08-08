#ifndef DEF_MAIN_H
#define DEF_MAIN_H

#include <QTranslator>

enum screenViewEnum{SCREEN_VIEW_SELECT_TRIBE, SCREEN_VIEW_SELECT_LOCATION, SCREEN_VIEW_RESOURCES};

extern screenViewEnum screenView;
extern QTranslator translator;

#endif
