#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "QPlus.h"
#include "QHoverLabel.h"
#include "QHoverPushButton.h"
#include "resources.h"
#include "buildings.h"
#include "main.h"

// this doesn't work as expected why ?!
//#define MYTR(x) QT_TRANSLATE_NOOP("resources", x)

qint64 timestampVillageStart,
       timestampGameRestored; // maybe can accumulate fraction of second errors

/*quint32 lumberProduction = 58, // should use an array ?
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
        initialCropAmount = 0;*/

quint32 resourcesProduction[] = {58, 52, 48, 56},
        resourcesAmount[] = {0, 0, 0, 0},
        initialResourcesAmount[] = {0, 0, 0, 0};

QString resourcesNames[] = {"lumber", "clay", "iron", "crop"};

quint8 farms[FARMS_NUMBER];

quint16 farmsScreen[FARMS_NUMBER][2] = {
    {738, 214},
    {850, 219},
    {935, 230},

    {700, 258},
    {825, 270},
    {877, 276},
    {972, 283},

    {655, 311},
    {717, 308},
    {950, 306},
    {1003, 300},

    {650, 366},
    {717, 353},
    {977, 365},
    {839, 389},

    {747, 443},
    {847, 438},
    {935, 424}
};

void setResourcesScreen(/*MyWindow* window*/)
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

    // tabs icons

    QString tabsAssets = "tabs/";
    QLabel* qResources = getQLabel(tabsAssets + "resourcesHover", true, QT_TRANSLATE_NOOP("resources", "resources"), "resources");
    //QPushButton* qBuildings = new QHoverPushButton(tabsAssets + QT_TRANSLATE_NOOP("resources", "buildings"), "buildings");
    QHoverLabel* qBuildings = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "buildings"), "buildings"),
               * qMap = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "map"), "map"),
               * qStatistics = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "statistics"), "statistics"),
               * qReports = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "reports"), "reports"),
               * qMessages = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "messages"), "messages");

    qTopHBox->addStretch();

    qTabsHBox->addWidget(qResources);
    qTabsHBox->addWidget(qBuildings);
    qTabsHBox->addWidget(qMap);
    qTabsHBox->addWidget(qStatistics);
    qTabsHBox->addWidget(qReports);
    qTabsHBox->addWidget(qMessages);

    //connect(qBuildings, SIGNAL(clicked()), window, [](){setBuildingsScreen();});
    //QObject::connect(qBuildings, &QPushButton::clicked, [](){qDebug() << "ya";});

    QObject::connect(qBuildings, &QHoverLabel::clicked, [](){ setBuildingsScreen(); }); // doesn't work if qMap is just QLabel* type

    qTabsIcons->setLayout(qTabsHBox);

    setColor(qTabsIcons, QColor(165, 147, 128));

    qTopHBox->addWidget(qTabsIcons);

    qTopHBox->addStretch();
    qTop->setLayout(qTopHBox);

    // resources
    qResourcesHBox->addStretch();

    updateResourcesAmount();

    addResourceCapacity(qResourcesHBox, QT_TRANSLATE_NOOP("resources", "warehouse"), 800);
    for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER - 1; resourcesIndex++)
    {
        addResource(qResourcesHBox, QT_TRANSLATE_NOOP("resources", resourcesNames[resourcesIndex]), resourcesAmount[resourcesIndex]);
    }
    addResourceCapacity(qResourcesHBox, QT_TRANSLATE_NOOP("resources", "granary"), 800);
    addResource(qResourcesHBox, QT_TRANSLATE_NOOP("resources", resourcesNames[CROP]), resourcesAmount[CROP]);

    qResourcesHBox->addStretch();
    qTabsResources->setLayout(qResourcesHBox);

    // resources production and troops
    qResourcesProductionTroopsHBox->addStretch(3);
    QColor backgroundResourcesProductionTroops = QColor(245, 247, 237);

    // ressources production

    setColor(qResourcesProduction, backgroundResourcesProductionTroops);
    qResourcesProductionVBox->addWidget(new QLabel(QObject::tr("Production per hour") + ":"));
    for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
    {
        addProductionLine(qResourcesProductionVBox, resourcesNames[resourcesIndex], resourcesProduction[resourcesIndex]);
    }
    qResourcesProduction->setLayout(qResourcesProductionVBox);

    // troops
    setColor(qTroops, backgroundResourcesProductionTroops);
    qTroopsVBox->addWidget(new QLabel(QObject::tr("Troops") + ":"));
    addTroopLine(qTroopsVBox, QT_TRANSLATE_NOOP("resources", "hero"), 1);
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

    resourceCapacityHBox->addWidget(getQLabel("resources/" + name, true, name, "resources"));
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
    hbox->addWidget(getQLabel("resources/" + name));
    hbox->addWidget(new QLabel(firstUppercase(translator.translate("resources", name.toStdString().c_str()) + ":")));
    hbox->addStretch();
    hbox->addWidget(new QLabel(QString::number(production)));
    productionLine->setLayout(hbox);
    qResourcesProductionVBox->addWidget(productionLine);
}

void addTroopLine(QVBoxLayout* qTroopsVBox, QString name, quint32 amount)
{
    QWidget* troopLine = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(getQLabel("troops/" + name + ".png")); // QT_TR_N_NOOP()
    //qInfo(name.toStdString().c_str());
    hbox->addWidget(new QLabel(QString::number(amount) + " " + firstUppercase(translator.translate("resources", name.toStdString().c_str()))));
    //qInfo(QObject::tr(name.toStdString().c_str()).toStdString().c_str());
    //qInfo(translator.tr(name.toStdString().c_str()).toStdString().c_str());
    //qInfo(translator.translate("resources", name.toStdString().c_str()).toStdString().c_str());
    hbox->addStretch();
    troopLine->setLayout(hbox);
    qTroopsVBox->addWidget(troopLine);
}

void updateResourcesAmount()
{
    double timeDelta = QDateTime::currentSecsSinceEpoch() - timestampGameRestored,
           timeFraction = timeDelta / 3600;
    for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
    {
        resourcesAmount[resourcesIndex] = initialResourcesAmount[resourcesIndex] + resourcesProduction[resourcesIndex] * timeFraction;
    }
}

void updateScreen(/*MyWindow* window*/)
{
    updateResourcesAmount();
    setResourcesScreen(/*window*/);
    return;

    //return;
    // TODO: could at least update screen only when resources amount change etc - otherwise have tooltip problem

    QWidget* centralWidget = window->centralWidget();
    QLayout* layout = centralWidget->layout();
    //if(layout == nullptr)
    //    qInfo("huge layout error");
    //qInfo(std::to_string(layout->count()).c_str());
    //qInfo("here");
    QLayoutItem* layoutItem = layout->itemAt(1);
    //for(qint8 i = 0; i < layout->count(); i++)
    //{
    //    qInfo(std::to_string(i).c_str());
    //    qInfo(layout->itemAt(i)->layout() != nullptr ? "layout" : "not layout");
    //}
    //if(layoutItem == 0)
    //    qInfo("first layout error");
    //qInfo("da");
    QLayout* subLayout = layoutItem->layout();
    //if(subLayout == nullptr)
    //    qInfo("sub layout error");
    //qInfo(std::to_string(subLayout->count()).c_str());
    QLayoutItem* subLayoutItem = subLayout->itemAt(2); // is a Strecth a QLayoutItem ? - it counts
    //if(subLayoutItem == 0)
    //    qInfo("layout error");
    QWidget* oldQWidget = subLayoutItem->widget();
    QWidget* qWidget = getResource(resourcesNames[LUMBER], resourcesAmount[LUMBER]);
    //if(layout->replaceWidget(oldQWidget, qWidget) == nullptr)
    //    qInfo("error");

    /*addResourceCapacity(qResourcesHBox, QT_TR_NOOP("warehouse"), 800);
    addResource(qResourcesHBox, QT_TR_NOOP("lumber"), lumberAmount);
    addResource(qResourcesHBox, QT_TR_NOOP("clay"), clayAmount);
    addResource(qResourcesHBox, QT_TR_NOOP("iron"), ironAmount);
    addResourceCapacity(qResourcesHBox, QT_TR_NOOP("granary"), 800);
    addResource(qResourcesHBox, QT_TR_NOOP("crop"), cropAmount);*/
}
