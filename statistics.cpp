#include "statistics.h"
#include "main.h"
#include "QPlus.h"
#include "resources.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QGridLayout>

void seeProfile()
{
    screenView = SCREEN_VIEW_PROFILE;
}

// we should also make a profile page for each user
QWidget* getStatisticsScreen()
{
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
    quint8 offset = 1; // 0
    //playersGrid->addWidget(new QLabel(QObject::tr("Alliance")), 0, 3);
    playersGrid->addWidget(new QLabel(QObject::tr("Population")), 0, 4 - offset);
    playersGrid->addWidget(new QLabel(QObject::tr("Village")), 0, 5 - offset);

    playersGrid->addWidget(new QLabel(QString::number(1) + "."), 1, 0);
    playersGrid->addWidget(getQLabel("tribes/" + getTribe() + " medium.png"), 1, 1);
    QPushButton* profileButton = new QPushButton(nickname); // would be cool to make text clickable

    QObject::connect(profileButton, &QPushButton::clicked, [](){ seeProfile(); });
    //playersGrid->addWidget(new QLabel("<a href=\"\">" + nickname + "</a>"), 1, 2); // href="google.com" doesn't work - we will see design when firstly we will have fonctionnality
    playersGrid->addWidget(profileButton, 1, 2);
    //playersGrid->addWidget(new QLabel("-"), 1, 3);
    playersGrid->addWidget(new QLabel(QString::number(population)), 1, 4 - offset);
    playersGrid->addWidget(new QLabel(QString::number(1)), 1, 5 - offset);

    playersTabVBox->addWidget(new QLabel(QObject::tr("The largest players")));
    playersTabGridContent->setLayout(playersGrid);
    playersTabVBox->addWidget(playersTabGridContent);
    playersTabVBox->addStretch();
    playersTabContent->setLayout(playersTabVBox);

    playersTab->addTab(playersTabContent, QObject::tr("Overview"));
    statisticsTabs->addTab(playersTab, QObject::tr("Players"));

    vbox->addWidget(statistics);
    vbox->addWidget(statisticsTabs);
    //vbox->addStretch();
    screen->setLayout(vbox);
    QColor backgroundCurrentlyBuilding = QColor(210, 189, 161);
    setColor(screen, backgroundCurrentlyBuilding);

    return screen;
}

void setStatisticsScreen()
{
    screenView = SCREEN_VIEW_STATISTICS;

    //QWidget* screen = getStatisticsScreen();

    //window->setCentralWidget(screen);
}
