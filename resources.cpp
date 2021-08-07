#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "QPlus.h"
#include "QHoverLabel.h"
#include "resources.h"

void setResourcesScreen(MyWindow* window)
{
    QWidget* screen = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout;
    QHBoxLayout* hbox = new QHBoxLayout;

    QWidget* qScreen = new QWidget; /*used to use: getQLabel(QT_TR_NOOP("resources"), true)*/
    QLabel* qResources = new QHoverLabel(QT_TR_NOOP("resources")), // Qt doesn't say when file not found :'(
          * qBuildings = new QHoverLabel(QT_TR_NOOP("buildings")),
          * qMap = new QHoverLabel(QT_TR_NOOP("map")),
          * qStatistics = new QHoverLabel(QT_TR_NOOP("statistics")),
          * qReports = new QHoverLabel(QT_TR_NOOP("reports")),
          * qMessages = new QHoverLabel(QT_TR_NOOP("messages"));

    hbox->addStretch(); // hopefully this works
    hbox->addWidget(qResources);
    hbox->addWidget(qBuildings);
    hbox->addWidget(qMap);
    hbox->addWidget(qStatistics);
    hbox->addWidget(qReports);
    hbox->addWidget(qMessages);
    hbox->addStretch();

    qScreen->setLayout(hbox);
    vbox->addWidget(qScreen);
    vbox->addWidget(qScreen);
    vbox->addStretch();

    screen->setLayout(vbox);

    window->setCentralWidget(screen);
}
