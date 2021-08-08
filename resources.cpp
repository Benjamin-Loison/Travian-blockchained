#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "QPlus.h"
#include "QHoverLabel.h"
#include "resources.h"
#include "main.h"

qint64 timestampVillageStart,
       timestampGameRestored; // maybe can accumulate fraction of second errors

quint32 lumberProduction = 58, // should use an array ?
        clayProduction = 52,
        ironProduction = 48,
        cropProduction = 56,

        lumberAmount = 0, // 750 all by default
        clayAmount = 0,
        ironAmount = 0,
        cropAmount = 0,

        initialLumberAmount = 0,
        initialClayAmount = 0,
        initialIronAmount = 0,
        initialCropAmount = 0;

void setResourcesScreen(MyWindow* window)
{
    QVBoxLayout* vbox = new QVBoxLayout,
               * qResourcesProductionTroopsVBox = new QVBoxLayout,
               * qResourcesProductionVBox = new QVBoxLayout,
               * qTroopsVBox = new QVBoxLayout;
    QHBoxLayout* qTopHBox = new QHBoxLayout,
               * qTabsHBox = new QHBoxLayout,
               * qResourcesHBox = new QHBoxLayout,
               * qResourcesProductionTroopsHBox = new QHBoxLayout;

    QWidget* screen = new QWidget,
           * qTop = new QWidget,
           * qTabsIcons = new QWidget,
           * qTabsResources = new QWidget,
           * qResourcesProductionTroops = new QWidget,
           * qResourcesProductionTroopsForBox = new QWidget,
           * qResourcesProduction = new QWidget,
           * qTroops = new QWidget;
    QLabel* qResources = getQLabel("resourcesHover", true, QObject::tr("resources")),
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

    updateResourcesAmount();

    addResourceCapacity(qResourcesHBox, QT_TR_NOOP("warehouse"), 800);
    addResource(qResourcesHBox, QT_TR_NOOP("lumber"), lumberAmount);
    addResource(qResourcesHBox, QT_TR_NOOP("clay"), clayAmount);
    addResource(qResourcesHBox, QT_TR_NOOP("iron"), ironAmount);
    addResourceCapacity(qResourcesHBox, QT_TR_NOOP("granary"), 800);
    addResource(qResourcesHBox, QT_TR_NOOP("crop"), cropAmount);

    qResourcesHBox->addStretch();
    qTabsResources->setLayout(qResourcesHBox);

    // resources production and troops
    qResourcesProductionTroopsHBox->addStretch(3);
    QColor backgroundResourcesProductionTroops = QColor(245, 247, 237);

    // ressources production

    setColor(qResourcesProduction, backgroundResourcesProductionTroops);
    qResourcesProductionVBox->addWidget(new QLabel(QObject::tr("Production per hour") + ":"));
    addProductionLine(qResourcesProductionVBox, "lumber", lumberProduction);
    addProductionLine(qResourcesProductionVBox, "clay", clayProduction);
    addProductionLine(qResourcesProductionVBox, "iron", ironProduction);
    addProductionLine(qResourcesProductionVBox, "crop", cropProduction);
    qResourcesProduction->setLayout(qResourcesProductionVBox);

    // troops
    setColor(qTroops, backgroundResourcesProductionTroops);
    qTroopsVBox->addWidget(new QLabel(QObject::tr("Troops") + ":"));
    addTroopLine(qTroopsVBox, QT_TR_NOOP("hero"), 1);
    qTroops->setLayout(qTroopsVBox);

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

QWidget* getResourceInfo(QString name, quint32 capacity, QColor backgroundColor, QColor foregroundColor)
{
    QWidget* resourceCapacity = new QWidget;
    QHBoxLayout* resourceCapacityHBox = new QHBoxLayout;

    resourceCapacityHBox->addWidget(getQLabel(name, true, name));
    QLabel* qLabel = new QLabel(std::to_string(capacity).c_str());
    //qLabel->setFont(QFont("Impact", 12));

    resourceCapacityHBox->addWidget(qLabel);

    resourceCapacity->setLayout(resourceCapacityHBox);
    setColor(resourceCapacity, backgroundColor, foregroundColor);
    return resourceCapacity;
}

void addResourceInfo(QHBoxLayout* hbox, QString name, quint32 capacity, QColor backgroundColor, QColor foregroundColor)
{
    QWidget* resourceCapacity = getResourceInfo(name, capacity, backgroundColor, foregroundColor);
    hbox->addWidget(resourceCapacity);
}

void addResourceCapacity(QHBoxLayout* hbox, QString name, quint32 capacity)
{
    addResourceInfo(hbox, name, capacity, QColor(102, 78, 64), Qt::white);
}

QWidget* getResource(QString name, quint32 amount)
{
    QWidget* qWidget = getResourceInfo(name, amount, QColor(227, 212, 188), Qt::black);
    return qWidget;
}

void addResource(QHBoxLayout* hbox, QString name, quint32 amount)
{
    QWidget* qWidget = getResource(name, amount);
    hbox->addWidget(qWidget);
    //addResourceInfo(hbox, name, amount, QColor(227, 212, 188), Qt::black);
    //addResourceCapacity(hbox, name, amount); // will soonly have to customize a bit
    //hbox->addWidget(getQLabel(name, true));
    //hbox->addWidget(new QLabel(std::to_string(amount).c_str()));
}

void addProductionLine(QVBoxLayout* qResourcesProductionVBox, QString name, quint32 production)
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

void addTroopLine(QVBoxLayout* qTroopsVBox, QString name, quint32 amount)
{
    QWidget* troopLine = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(getQLabel(name + ".png"));
    hbox->addWidget(new QLabel(QString::number(amount) + " " + firstUppercase(QObject::tr(name.toStdString().c_str()))));
    hbox->addStretch();
    troopLine->setLayout(hbox);
    qTroopsVBox->addWidget(troopLine);
}

void updateResourcesAmount()
{
    double timeDelta = QDateTime::currentSecsSinceEpoch() - timestampGameRestored,
           timeFraction = timeDelta / 3600;
    lumberAmount = initialLumberAmount + lumberProduction * timeFraction;
    clayAmount = initialClayAmount + clayProduction * timeFraction;
    ironAmount = initialIronAmount + ironProduction * timeFraction;
    cropAmount = initialCropAmount + cropProduction * timeFraction;
}

void updateScreen(MyWindow* window)
{
    //setResourcesScreen(window);

    updateResourcesAmount();

    QWidget* centralWidget = window->centralWidget();
    QLayout* layout = centralWidget->layout();
    if(layout == nullptr)
        qInfo("huge layout error");
    //qInfo(std::to_string(layout->count()).c_str());
    qInfo("here");
    QLayoutItem* layoutItem = layout->itemAt(1);
    for(qint8 i = 0; i < layout->count(); i++)
    {
        qInfo(std::to_string(i).c_str());
        qInfo(layout->itemAt(i)->layout() != nullptr ? "layout" : "not layout");
    }
    if(layoutItem == 0)
        qInfo("first layout error");
    qInfo("da");
    QLayout* subLayout = layoutItem->layout();
    if(subLayout == nullptr)
        qInfo("sub layout error");
    qInfo(std::to_string(subLayout->count()).c_str());
    QLayoutItem* subLayoutItem = subLayout->itemAt(2); // is a Strecth a QLayoutItem ? - it counts
    if(subLayoutItem == 0)
        qInfo("layout error");
    QWidget* oldQWidget = subLayoutItem->widget();
    QWidget* qWidget = getResource("lumber", lumberAmount);
    if(layout->replaceWidget(oldQWidget, qWidget) == nullptr)
        qInfo("error");

    /*addResourceCapacity(qResourcesHBox, QT_TR_NOOP("warehouse"), 800);
    addResource(qResourcesHBox, QT_TR_NOOP("lumber"), lumberAmount);
    addResource(qResourcesHBox, QT_TR_NOOP("clay"), clayAmount);
    addResource(qResourcesHBox, QT_TR_NOOP("iron"), ironAmount);
    addResourceCapacity(qResourcesHBox, QT_TR_NOOP("granary"), 800);
    addResource(qResourcesHBox, QT_TR_NOOP("crop"), cropAmount);*/
}
