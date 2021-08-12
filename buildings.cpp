#include "main.h"
#include "QPlus.h"
#include "buildings.h"

void setBuildingsScreen()
{
    screenView = SCREEN_VIEW_BUILDINGS;
    //window->update();
    window->manageBackground();
    return;
    QWidget* screen = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout,
               * playersTabVBox = new QVBoxLayout;
    QLabel* statistics = new QLabel(QObject::tr("Statistics"));
    QTabWidget* statisticsTabs = new QTabWidget,
              * playersTab = new QTabWidget;
    QWidget* playersTabContent = new QWidget,
           * playersTabGridContent = new QWidget;
    QGridLayout* playersGrid = new QGridLayout; // could add borders lines
    playersGrid->addWidget(new QLabel(QObject::tr("Players")), 0, 2);
    playersGrid->addWidget(new QLabel(QObject::tr("Alliance")), 0, 3);
    playersGrid->addWidget(new QLabel(QObject::tr("Population")), 0, 4);
    playersGrid->addWidget(new QLabel(QObject::tr("Village")), 0, 5);

    playersGrid->addWidget(new QLabel(QString::number(1) + "."), 1, 0);
    playersGrid->addWidget(getQLabel("tribes/gauls medium.png"), 1, 1);
    playersGrid->addWidget(new QLabel("Benjamin Loison"), 1, 2);
    playersGrid->addWidget(new QLabel("-"), 1, 3);
    playersGrid->addWidget(new QLabel(QString::number(8)), 1, 4);
    playersGrid->addWidget(new QLabel(QString::number(1)), 1, 5);

    playersTabVBox->addWidget(new QLabel(QObject::tr("The largest players")));
    playersTabGridContent->setLayout(playersGrid);
    playersTabVBox->addWidget(playersTabGridContent);
    playersTabVBox->addStretch();
    playersTabContent->setLayout(playersTabVBox);

    playersTab->addTab(playersTabContent, QObject::tr("Overview"));
    statisticsTabs->addTab(playersTab, QObject::tr("Players"));

    vbox->addWidget(statistics);
    vbox->addWidget(statisticsTabs);
    vbox->addStretch();
    screen->setLayout(vbox);

    window->setCentralWidget(screen);
}
