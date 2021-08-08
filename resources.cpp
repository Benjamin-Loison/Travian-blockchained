#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "QPlus.h"
#include "QHoverLabel.h"
#include "resources.h"

void setResourcesScreen(MyWindow* window)
{
    QWidget* screen = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout,
               * qResourcesProductionTroopsVBox = new QVBoxLayout,
               * qResourcesProductionVBox = new QVBoxLayout,
               * qTroopsVBox = new QVBoxLayout;
    QHBoxLayout* qTopHBox = new QHBoxLayout,
               * qTabsHBox = new QHBoxLayout,
               * qResourcesHBox = new QHBoxLayout,
               * qResourcesProductionTroopsHBox = new QHBoxLayout;

    QWidget* qTop = new QWidget,
           * qTabsIcons = new QWidget,
           * qTabsResources = new QWidget,
           * qResourcesProductionTroops = new QWidget,
           * qResourcesProductionTroopsForBox = new QWidget,
           * qResourcesProduction = new QWidget,
           * qTroops = new QWidget;
    QLabel* qResources = getQLabel("resourcesHover", true, QT_TR_NOOP("resources")),
          * qBuildings = new QHoverLabel(QT_TR_NOOP("buildings")),
          * qMap = new QHoverLabel(QT_TR_NOOP("map")),
          * qStatistics = new QHoverLabel(QT_TR_NOOP("statistics")),
          * qReports = new QHoverLabel(QT_TR_NOOP("reports")),
          * qMessages = new QHoverLabel(QT_TR_NOOP("messages"));

    // tabs icons
    qTopHBox->addStretch();

    qTabsHBox->addWidget(qResources);
    qTabsHBox->addWidget(qBuildings);
    qTabsHBox->addWidget(qMap);
    qTabsHBox->addWidget(qStatistics);
    qTabsHBox->addWidget(qReports);
    qTabsHBox->addWidget(qMessages);

    qTabsIcons->setLayout(qTabsHBox);

    setColor(qTabsIcons, QColor(165, 147, 128));

    qTopHBox->addWidget(qTabsIcons);

    qTopHBox->addStretch();
    qTop->setLayout(qTopHBox);

    // resources
    qResourcesHBox->addStretch();

    addResourceCapacity(qResourcesHBox, QT_TR_NOOP("warehouse"), 800);
    addResource(qResourcesHBox, QT_TR_NOOP("lumber"), 750);
    addResource(qResourcesHBox, QT_TR_NOOP("clay"), 750);
    addResource(qResourcesHBox, QT_TR_NOOP("iron"), 750);
    addResourceCapacity(qResourcesHBox, QT_TR_NOOP("granary"), 800);
    addResource(qResourcesHBox, QT_TR_NOOP("crop"), 750);

    qResourcesHBox->addStretch();
    qTabsResources->setLayout(qResourcesHBox);

    // resources production and troops
    qResourcesProductionTroopsHBox->addStretch(3);
    QColor backgroundResourcesProductionTroops = QColor(245, 247, 237);

    // ressources production

    setColor(qResourcesProduction, backgroundResourcesProductionTroops);
    qResourcesProductionVBox->addWidget(new QLabel(QObject::tr("Production per hour") + ":"));
    addProductionLine(qResourcesProductionVBox, "lumber", 58);
    addProductionLine(qResourcesProductionVBox, "clay", 52);
    addProductionLine(qResourcesProductionVBox, "iron", 48);
    addProductionLine(qResourcesProductionVBox, "crop", 56);
    qResourcesProduction->setLayout(qResourcesProductionVBox);

    // troops
    setColor(qTroops, backgroundResourcesProductionTroops);
    qTroopsVBox->addWidget(new QLabel(QObject::tr("Troops") + ":"));
    addTroopLine(qTroopsVBox, QT_TR_NOOP("hero"), 1);
    qTroops->setLayout(qTroopsVBox);


    /*qResourcesProductionTroopsVBox->addWidget(qResourcesProduction);
    qResourcesProductionTroopsVBox->addWidget(qTroops);
    qResourcesProductionTroops->setLayout(qResourcesProductionTroopsVBox);

    qResourcesProductionTroopsHBox->addWidget(qResourcesProductionTroops);
    qResourcesProductionTroopsHBox->addStretch(2);
    qResourcesProductionTroopsForBox->setLayout(qResourcesProductionTroopsHBox);*/


    qResourcesProductionTroopsVBox->addWidget(qResourcesProduction);
    qResourcesProductionTroopsVBox->addWidget(qTroops);
    qResourcesProductionTroops->setLayout(qResourcesProductionTroopsVBox);

    qResourcesProductionTroopsHBox->addWidget(qResourcesProductionTroops);
    qResourcesProductionTroopsHBox->addStretch(2);
    qResourcesProductionTroopsForBox->setLayout(qResourcesProductionTroopsHBox);

    // assemble this into vertical boxes

    vbox->addWidget(qTop);
    vbox->addWidget(qTabsResources);
    vbox->addStretch(1);
    vbox->addWidget(qResourcesProductionTroopsForBox);
    vbox->addStretch(2);

    screen->setLayout(vbox);

    window->setCentralWidget(screen);
}

void addResourceInfo(QHBoxLayout* hbox, QString name, unsigned int capacity, QColor backgroundColor, QColor foregroundColor)
{
    QWidget* resourceCapacity = new QWidget;
    QHBoxLayout* resourceCapacityHBox = new QHBoxLayout;

    resourceCapacityHBox->addWidget(getQLabel(name, true));
    QLabel* qLabel = new QLabel(std::to_string(capacity).c_str());
    //qLabel->setFont(QFont("Impact", 12));

    resourceCapacityHBox->addWidget(qLabel);

    resourceCapacity->setLayout(resourceCapacityHBox);
    setColor(resourceCapacity, backgroundColor, foregroundColor);
    hbox->addWidget(resourceCapacity);
}

void addResourceCapacity(QHBoxLayout* hbox, QString name, unsigned int capacity)
{
    addResourceInfo(hbox, name, capacity, QColor(102, 78, 64), Qt::white);
}

void addResource(QHBoxLayout* hbox, QString name, unsigned int amount)
{
    addResourceInfo(hbox, name, amount, QColor(227, 212, 188), Qt::black);
    //addResourceCapacity(hbox, name, amount); // will soonly have to customize a bit
    //hbox->addWidget(getQLabel(name, true));
    //hbox->addWidget(new QLabel(std::to_string(amount).c_str()));
}

void addProductionLine(QVBoxLayout* qResourcesProductionVBox, QString name, unsigned int production)
{
    QWidget* productionLine = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(getQLabel(name));
    hbox->addWidget(new QLabel(firstUppercase(QObject::tr(name.toStdString().c_str()) + ":")));
    hbox->addStretch();
    hbox->addWidget(new QLabel(QString::number(production)));
    productionLine->setLayout(hbox);
    qResourcesProductionVBox->addWidget(productionLine);
}

void addTroopLine(QVBoxLayout* qTroopsVBox, QString name, unsigned int amount)
{
    QWidget* troopLine = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(getQLabel(name + ".png"));
    hbox->addWidget(new QLabel(QString::number(amount) + " " + firstUppercase(QObject::tr(name.toStdString().c_str()))));
    hbox->addStretch();
    troopLine->setLayout(hbox);
    qTroopsVBox->addWidget(troopLine);
}
