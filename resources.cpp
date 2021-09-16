#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "QPlus.h"
#include "QHoverLabel.h"
#include "QHoverPushButton.h"
#include "QClickableWidget.h"
#include "resources.h"
#include "buildings.h"
#include "map.h"
#include "statistics.h"
#include "reports.h"
#include "profile.h"
#include "village.h"
#include "main.h"
#include "MyWindow.h"
#include "QRandomPlus.h"
#include "CryptoPlus.h"
#include "QFilePlus.h"

// this doesn't work as expected why ?!
//#define MYTR(x) QT_TRANSLATE_NOOP("resources", x)

quint32 timestampVillageStart,
        timestampGameClosed, // maybe can accumulate fraction of second errors
        lastActionTime,
        warehouseCapacity = 800,
        granaryCapacity = 800,
        population,
        culturePoints;

// could store as splitted values but might have to give more or less information for various actions (town building or troops formation for instance)
QString lastAction;

quint32 resourcesProduction[] = {44, 32, 48, 56/*0, 0, 0, 0*/},
        resourcesAmount[] = {0, 0, 0, 0, 0, 0},
        initialResourcesAmount[] = {0, 0, 0, 0};

// culture point are the same for each farm type but let's have a more general approach

//quint8 culturePointsFarms[FARMS_MAX_LEVEL] = {1, 1, 2, 2, 2, 3, 4, 4, 5, 6, 7, 9, 11, 13, 15, 18, 22, 27, 32, 38};

quint16 theoricProduction[] = {3, 7, 13, 21, 31, 46, 70, 98, 140, 203, 280, 392, 525, 693, 889, 1120, 1400, 1820, 2240, 2800, 3430};

QString resourcesNames[RESOURCES_NUMBER + 1] = {QT_TRANSLATE_NOOP("resources", "lumber"),
                                                QT_TRANSLATE_NOOP("resources", "clay"),
                                                QT_TRANSLATE_NOOP("resources", "iron"),
                                                QT_TRANSLATE_NOOP("resources", "crop"),
                                                QT_TRANSLATE_NOOP("resources", "free crop")};

QString farmsNames[RESOURCES_NUMBER] = {QT_TRANSLATE_NOOP("resources", "woodcutter"),
                                        QT_TRANSLATE_NOOP("resources", "clay pit"),
                                        QT_TRANSLATE_NOOP("resources", "iron mine"),
                                        QT_TRANSLATE_NOOP("resources", "cropland")};

quint8 farms[FARMS_NUMBER];

quint8 farmsTypes[FARMS_NUMBER] = {
    0, 3, 0,
    2, 1, 1, 2,
    3, 3, 2, 2,
    3, 3, 0, 3,
    1, 0, 1
};

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
    {839, 389},
    {977, 365},

    {747, 443},
    {847, 438},
    {935, 424}
};

quint8 getResourceIndexFromName(QString resourceName)
{
    if(resourceName == "lumber") return 0;
    else if(resourceName == "clay") return 1;
    else if(resourceName == "iron") return 2;
    else if(resourceName == "iron") return 3;
    else return QUINT8_MAX_VALUE;
}

void setResourceScreen()
{
    screenView = SCREEN_VIEW_RESOURCES;
    myWindow->manageBackground();
}

void buildingGetBuilt(quint8 farmsIndex)
{
    quint8 farmLevel = farms[farmsIndex],
           farmType = farmsTypes[farmsIndex];
    farms[farmsIndex]++;
    culturePoints += buildingsCosts[farmType][farmLevel + 1][CULTURE_POINTS];
    resourcesProduction[farmType] += theoricProduction[farmLevel + 1] - theoricProduction[farmLevel];
    //qInfo(QString::number(resourcesProduction[farmType]).toStdString().c_str());
    myWindow->manageBackground();
}

void upgradeResource()
{
    quint8 farmLevel = farms[screenViewFarmsIndex],
           farmType = farmsTypes[screenViewFarmsIndex];
    for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++) // TODO check before display if have enough resources
    {
        initialResourcesAmount[resourcesIndex] = resourcesAmount[resourcesIndex] - buildingsCosts[farmType][farmLevel + 1][resourcesIndex];
    }
    resourcesAmount[FREE_CROP] -= buildingsCosts[farmType][farmLevel + 1][FREE_CROP];
    population += buildingsCosts[farmType][farmLevel + 1][FREE_CROP];
    // could use a orange background for circle farm when it is getting built

    timestampGameClosed = getSeconds();

    // what is the signature file format ? if use SHA256 have 70 "wc characters" and only 72 if use SHA512 - maybe word count only counts ascii characters
    QString nonce = randomHex(512),
            action = "farms:" + QString::number(screenViewFarmsIndex),
            hash = doubleSHA512(QString::number(timestampGameClosed) + SEPARATOR + action + SEPARATOR + nonce),
            filePath = USER_FOLDER + "tmp.txt",
            signedFilePath = filePath + ".sha512";
    writeFile(filePath, hash);
    callOpenSSL("dgst -sha512 -sign " + PRIVATE_KEY + " -out " + signedFilePath + " " + filePath);
    removeFile(filePath);
    QString signature = getFileContentAsHex(signedFilePath);
    removeFile(signedFilePath);
    saveAction(timestampGameClosed, action, nonce, signature);

    setResourceScreen();
}

quint32 secondsBeforeHaving(quint8 resourcesIndex, quint32 resourceAmount)
{
    quint32 currentResourceAmount = resourcesAmount[resourcesIndex];
    return resourceAmount <= currentResourceAmount ? 0 : (resourceAmount - currentResourceAmount) / (resourcesProduction[resourcesIndex] / 3600.0);
}

void addWidgetCulturePopulation(QHBoxLayout* hbox, QString name, quint8 currentAmount, quint8 addAmount)
{
    //hbox->addStretch();
    hbox->addWidget(getQLabel("resources/" + name + ".png")); // getResource("culture point", 0, false)
    //hbox->addStretch();
    QLabel* qCurrentInfo = new QLabel(QString::number(currentAmount));
    hbox->addWidget(qCurrentInfo);
    QLabel* qAddingInfo = new QLabel("(+" + QString::number(addAmount) + ")");
    QPalette palette = qAddingInfo->palette();
    palette.setColor(qAddingInfo->foregroundRole(), /*Qt::green*/QColor(34, 139, 34));
    qAddingInfo->setPalette(palette);
    hbox->addWidget(qAddingInfo);
}

QWidget* getImproveScreen(quint8 buildingsIndex, QString context, quint8 level, QStringList lines, QString description, QList<QPair<QString, QWidget*>> tabs, QWidget* additionnalContent)
{
    QWidget* screen = new QWidget,
           * vWidget = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout;
    QHBoxLayout* hbox = new QHBoxLayout;

    bool isAFarm = context == "resources",
         isMonoTab = tabs.isEmpty();
    quint8 costsOffset = isAFarm ? 0 : RESOURCES_NUMBER;
    QString buildingName = isAFarm ? farmsNames[buildingsIndex] : buildingsNames[buildingsIndex],
            title = firstUppercase(translator.translate(context.toStdString().c_str(), buildingName.toStdString().c_str())) + " " + translator.translate("resources", "Level") + " " + QString::number(level);
    QLabel* qTitle = new QLabel(title);
    qTitle->setFont(QFont("Comic Sans MS", 20, QFont::Bold));
    vbox->addWidget(qTitle);

    QVBoxLayout* mainPageVBox = vbox;

    if(!isMonoTab)
    {
        QVBoxLayout* managementTabVBox = new QVBoxLayout;
        //free(vbox);
        mainPageVBox = managementTabVBox;
    }

    QLabel* qResourceBackground = new QLabel;
    QPixmap resourceBackground = getQPixmap("village/resourceBackground.jpg"),
            resource = getQPixmap((isAFarm ? "village" : "buildings/zoom/" + getTribe()) + "/" + buildingName + ".png");
    //qInfo(("!" + buildingName + "!").toStdString().c_str());
    QPainter* painter = new QPainter(&resourceBackground);
    painter->drawPixmap(319, 0, resource);
    QFont bold = QFont("Comic Sans MS", 13, QFont::Bold);
    painter->setFont(bold);
    quint8 linesLength = lines.length();
    for(quint8 linesIndex = 0; linesIndex < linesLength; linesIndex++)
    {
        // will have to vertically center when will have more than two lines
        QString line = lines[linesIndex];
        painter->drawText(10, 77 + 21 * linesIndex, line);
    }
    painter->end();
    qResourceBackground->setPixmap(resourceBackground);

    mainPageVBox->addWidget(qResourceBackground);
    QLabel* qDescription = new QLabel(description);
    qDescription->setWordWrap(true);
    mainPageVBox->addWidget(qDescription);
    quint8 maxLevel = isAFarm ? 20 : buildingsLevelsNumber[buildingsIndex] - 1;
    if(level < maxLevel)
    {
        QWidget* qResourcesRequired = new QWidget;
        QHBoxLayout* resourcesRequiredHBox = new QHBoxLayout;
        bool hasEnoughResources = true;
        quint32 maxSeconds = 0;
        QString notPossibleMessage = "";
        for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
        {
            quint32 farmCost = buildingsCosts[costsOffset + buildingsIndex][level + 1][resourcesIndex];
            if(resourcesIndex < 3)
            {
                 if(farmCost > warehouseCapacity)
                 {
                     notPossibleMessage = QObject::tr("Extended warehouse first.");
                 }
            }
            else if(resourcesIndex == 3 && farmCost > granaryCapacity)
            {
                notPossibleMessage = notPossibleMessage != "" ? QObject::tr("Extend warehouse and granary first.") : QObject::tr("Extended granary first.");
            }
            if(resourcesAmount[resourcesIndex] < farmCost)
            {
                hasEnoughResources = false;
                quint32 secondsBeforeHavingResource = secondsBeforeHaving(resourcesIndex, farmCost);
                maxSeconds = qMax(maxSeconds, secondsBeforeHavingResource);
            }
        }
        //resourcesRequiredHBox->addStretch();
        for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER + 1; resourcesIndex++)
        {
            QString resourceName = resourcesNames[resourcesIndex];
            quint32 cost = buildingsCosts[costsOffset + buildingsIndex][level + 1][resourcesIndex];
            QWidget* qResource = getResource(resourceName, cost, false);
            /*if(resourcesIndex == 0)
                qResource->setAlignment(Qt::AlignLeft);
            if(resourcesIndex == RESOURCES_NUMBER - 1)
                qResource->setAlignment(Qt::AlignRight);*/
            resourcesRequiredHBox->addWidget(qResource);
        }
        resourcesRequiredHBox->setAlignment(Qt::AlignHCenter);
        //resourcesRequiredHBox->addStretch();
        qResourcesRequired->setLayout(resourcesRequiredHBox);
        mainPageVBox->addWidget(qResourcesRequired);
        if(!hasEnoughResources && notPossibleMessage == "")
        {
            QDateTime enoughResourcesDateTime = getDateTime().addSecs(maxSeconds); // may look ridiculous but have to pay attention to time difference between countries etc
            //qInfo(("final: " + QString::number(maxSeconds)).toStdString().c_str());

            notPossibleMessage = QObject::tr("Enough resources on") + " " + enoughResourcesDateTime.toString("dd.MM.yy") + " " + QObject::tr("at") + " " + enoughResourcesDateTime.toString("hh:mm") + ".";
        }
        if(buildingsCosts[costsOffset + buildingsIndex][level + 1][FREE_CROP] > resourcesAmount[FREE_CROP]) // could make a function to access buildingsCosts precising with a boolean if we want the offset or not (ie if we consider a building or a farm)
        {
            notPossibleMessage = QObject::tr("Lack of food: extend cropland first !");
        }
        if(lastActionTime != 0)
        {
            notPossibleMessage = QObject::tr("Your masons are still working on another building !");
        }
        if(notPossibleMessage != "")
        {
            hasEnoughResources = false;
            QLabel* qNotPossibleMessage = new QLabel(notPossibleMessage);
            mainPageVBox->addWidget(qNotPossibleMessage);
        }

        QWidget* qCulturePopulation = new QWidget;
        QHBoxLayout* culturePopulationHBox = new QHBoxLayout;

        quint8 currentCulturePointAmount = buildingsCosts[costsOffset + buildingsIndex][level][CULTURE_POINTS],// 8 ? - yes max is chambre du trésor niveau 20 (230)
               addCulturePointAmount = buildingsCosts[costsOffset + buildingsIndex][level + 1][CULTURE_POINTS] - currentCulturePointAmount,

               currentPopulationAmount = 0,//buildingsCosts[costsOffset + buildingsIndex][level][FREE_CROP],
               addPopulationAmount = buildingsCosts[costsOffset + buildingsIndex][level + 1][FREE_CROP]; // it's not a relative but absolute value like for culture points
        for(quint8 levelsIndex = 0; levelsIndex < level + 1; levelsIndex++)
        {
            currentPopulationAmount += buildingsCosts[costsOffset + buildingsIndex][levelsIndex][FREE_CROP];
        }

        culturePopulationHBox->setAlignment(Qt::AlignHCenter);
        addWidgetCulturePopulation(culturePopulationHBox, "culture point", currentCulturePointAmount, addCulturePointAmount);
        addWidgetCulturePopulation(culturePopulationHBox, "population", currentPopulationAmount, addPopulationAmount);

        qCulturePopulation->setLayout(culturePopulationHBox);

        mainPageVBox->addWidget(qCulturePopulation);

        if(hasEnoughResources)
        {
            QWidget* qUpgrade = new QWidget;
            QHBoxLayout* upgradeHBox = new QHBoxLayout;
            QPushButton* upgradeButton = new QPushButton(QObject::tr("Upgrade to level") + " " + QString::number(level + 1));
            //upgradeHBox->addStretch();
            upgradeHBox->addWidget(upgradeButton);
            upgradeHBox->setAlignment(Qt::AlignHCenter);
            //upgradeHBox->addStretch();
            qUpgrade->setLayout(upgradeHBox);
            mainPageVBox->addWidget(qUpgrade);

            QObject::connect(upgradeButton, &QPushButton::clicked, [](){ upgradeResource(); });
        }

        QWidget* qTime = new QWidget;
        QHBoxLayout* timeHBox = new QHBoxLayout;
        timeHBox->setAlignment(Qt::AlignHCenter);
        timeHBox->addWidget(getQLabel("resources/time.png"));
        quint32 time = getTimeForBuilding(costsOffset + buildingsIndex, level + 1);
        QLabel* qCurrentInfo = new QLabel(getHoursMinutesSecondsStr(time));
        timeHBox->addWidget(qCurrentInfo);
        qTime->setLayout(timeHBox);
        mainPageVBox->addWidget(qTime);
    }
    if(additionnalContent != nullptr)
    {
        mainPageVBox->addWidget(additionnalContent);
    }

    if(!isMonoTab) // could be cool to have a fix width defined by first tab
    {
        QWidget* managementTab = new QWidget;
        QTabWidget* qTabs = new QTabWidget;
        managementTab->setLayout(mainPageVBox);
        qTabs->addTab(managementTab, QObject::tr("Management")); // or could just always show management for main tab
        quint8 tabsLength = tabs.length();
        for(quint8 tabsIndex = 0; tabsIndex < tabsLength; tabsIndex++)
        {
            QPair<QString, QWidget*> tab = tabs[tabsIndex];
            QString tabName = tab.first;
            QWidget* qTab = tab.second;
            qTabs->addTab(qTab, tabName);
        }
        vbox->addWidget(qTabs);
    }

    vWidget->setLayout(vbox);
    QPalette palette = vWidget->palette();
    palette.setColor(QPalette::Window, Qt::white);
    vWidget->setAutoFillBackground(true);
    vWidget->setPalette(palette);

    hbox->addStretch();
    hbox->addWidget(vWidget);
    hbox->addStretch();
    screen->setLayout(hbox);

    return screen;
}

QWidget* getResourceScreen()
{
    quint8 farmLevel = farms[screenViewFarmsIndex],
           farmType = farmsTypes[screenViewFarmsIndex];
    //QString resourceName = resourcesNames[farmType];
    QStringList lines;
    lines.append(QObject::tr("Current production:") + " " + QString::number(theoricProduction[farmLevel]) + " " + QObject::tr("per hour"));
    if(farmLevel < 20)
        lines.append(QObject::tr("Production at level") + " " + QString::number(farmLevel + 1) + ":" + " " + QString::number(theoricProduction[farmLevel + 1]) + " " + QObject::tr("per hour"));
    QString description;
    if(farmType == LUMBER)
        description = QObject::tr("The woodcutter cuts down trees in order to produce lumber. The further you extend the woodcutter, the more lumber is produced.\n\nBy constructing a sawmill, you can further increase the production.");
    else if(farmType == CLAY)
        description = QObject::tr("Here, clay is produced. By increasing its level, you increase clay production.\n\nBy constructing a brickyard, you can further increase the production.");
    else if(farmType == IRON)
        description = QObject::tr("Here, miners gather the precious resource of iron. By increasing the mine's level, you increase its iron production.\n\nBy constructing an iron foundry, you can further increase the production.");
    else if(farmType == CROP)
        description = QObject::tr("Your population's food is produced here. By increasing the cropland's level, you increase crop production.\n\nBy constructing a grain mill and a bakery, you can further increase the production.");
    return getImproveScreen(farmType, "resources", farmLevel, lines, description);
}

void setResourceFarmScreen(quint8 farmsIndex)
{
    screenView = SCREEN_VIEW_RESOURCE;
    screenViewFarmsIndex = farmsIndex;
}

QWidget* getCurrentlyBuildingWidget()
{
    if(lastActionTime != 0)
    {
        QStringList actionParts = lastAction.split(':');
        quint8 actionPartsLength = actionParts.length();
        if(actionPartsLength < 2) return nullptr; // will have to check if first part is "farms"
        quint8 farmsIndex = actionParts[1].toUInt(), // let's not use screenViewFarmsIndex not to have any conflict
               farmLevel = farms[farmsIndex] + 1,
               farmType = farmsTypes[farmsIndex];
        //qInfo((QString::number(farmsIndex)).toStdString().c_str());
        //qInfo((QString::number(farmLevel)).toStdString().c_str());

        quint32 timeRequired = getTimeForBuilding(farmType, farmLevel),
                currentTime = getSeconds();
        if(lastActionTime + timeRequired < currentTime)
        {
            lastActionTime = 0;
            buildingGetBuilt(farmsIndex);
            return nullptr;
        }
        quint32 remainingTime = timeRequired - (currentTime - lastActionTime);
        //qInfo((QString::number(timeRequired)).toStdString().c_str());
        //QDateTime finishedBuildingDateTime;
        //finishedBuildingDateTime.setSecsSinceEpoch(lastActionTime + timeRequired);
        QDateTime finishedBuildingDateTime = QDateTime::fromSecsSinceEpoch(lastActionTime + timeRequired);

        QString hoursMinutesSeconds = getHoursMinutesSecondsStr(remainingTime),
                constructingBuilding = firstUppercase(translator.translate("resources", resourcesNames[farmType].toStdString().c_str())) + " " + translator.translate("resources", "Level") + " " + QString::number(farmLevel),
                timeRemaining = hoursMinutesSeconds + " " + QObject::tr("hours done at") + " " + finishedBuildingDateTime.toString("hh:mm"); // in french version not hours written u_u
        QHBoxLayout* hbox = new QHBoxLayout//,
                   ;//* hboxBuilding = new QHBoxLayout;
        QVBoxLayout* vbox = new QVBoxLayout;
        QWidget* qHorizontalCurrentlyBuilding = new QWidget,
               * qCurrentlyBuilding = new QWidget//,
               ;//* qCurrentlyBuildingLine = new QWidget;
        //vbox->addStretch();
        vbox->addWidget(new QLabel(QObject::tr("Building:")));

        /*hboxBuilding->addWidget(new QLabel(constructingBuilding));
        hboxBuilding->addStretch(1);
        hboxBuilding->addWidget(new QLabel(timeRemaining));
        hboxBuilding->addStretch(1);
        qCurrentlyBuildingLine->setLayout(hboxBuilding);*/

        vbox->addWidget(new QLabel(constructingBuilding + "\t"/*" "*/ + timeRemaining)); // should display "Clay Pit" instead of Clay when we speak about the farm and not the resource it seems
        //vbox->addWidget(qCurrentlyBuildingLine);
        QColor backgroundCurrentlyBuilding = QColor(245, 247, 237);
        setColor(qCurrentlyBuilding, backgroundCurrentlyBuilding);
        qCurrentlyBuilding->setLayout(vbox);
        hbox->addStretch();
        hbox->addWidget(qCurrentlyBuilding);
        hbox->addStretch();
        qHorizontalCurrentlyBuilding->setLayout(hbox);
        return qHorizontalCurrentlyBuilding;
    }
    else
        return nullptr;
}

void setResourcesScreen()
{
    QVBoxLayout* vbox = new QVBoxLayout;
    QHBoxLayout* qTopHBox = new QHBoxLayout,
               * qTabsHBox = new QHBoxLayout,
               * qResourcesHBox = new QHBoxLayout;

    QClickableWidget* screen = new QClickableWidget;
    QWidget//* screen = new QWidget,
           * qTop = new QWidget,
           * qTabsIcons = new QWidget,
           * qTabsResources = new QWidget;

    // tabs icons

    QString tabsAssets = "tabs/";
    //qInfo(getScreenView().toStdString().c_str());
    //QLabel* qResources = getQLabel(tabsAssets + "resourcesHover", true, QT_TRANSLATE_NOOP("resources", "resources"), "resources");
    QHoverLabel* qResources = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "resources"), "resources", screenView == SCREEN_VIEW_RESOURCES),
               * qBuildings = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "buildings"), "buildings", screenView == SCREEN_VIEW_BUILDINGS),
               * qMap = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "map"), "map", screenView == SCREEN_VIEW_MAP),
               * qStatistics = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "statistics"), "statistics", screenView == SCREEN_VIEW_STATISTICS),
               * qReports = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "reports"), "reports", screenView == SCREEN_VIEW_REPORTS);//,
               //* qMessages = new QHoverLabel(tabsAssets + QT_TRANSLATE_NOOP("resources", "messages"), "messages", false);

    qTopHBox->addStretch();

    qTabsHBox->addWidget(qResources);
    qTabsHBox->addWidget(qBuildings);
    qTabsHBox->addWidget(qMap);
    qTabsHBox->addWidget(qStatistics);
    qTabsHBox->addWidget(qReports);
    //qTabsHBox->addWidget(qMessages);

    QObject::connect(qResources, &QHoverLabel::clicked, [](){ setResourceScreen(); });
    QObject::connect(qBuildings, &QHoverLabel::clicked, [](){ setBuildingsScreen(); });
    QObject::connect(qMap, &QHoverLabel::clicked, [](){ setMapScreen(); });
    QObject::connect(qStatistics, &QHoverLabel::clicked, [](){ setStatisticsScreen(); });
    QObject::connect(qReports, &QHoverLabel::clicked, [](){ setReportsScreen(); });

    qTabsIcons->setLayout(qTabsHBox);

    setColor(qTabsIcons, QColor(165, 147, 128));

    qTopHBox->addWidget(qTabsIcons);

    qTopHBox->addStretch();
    qTop->setLayout(qTopHBox);

    // resources
    qResourcesHBox->addStretch();

    updateResourcesAmount();

    addResourceCapacity(qResourcesHBox, QT_TRANSLATE_NOOP("resources", "warehouse"), warehouseCapacity);
    for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER - 1; resourcesIndex++)
    {
        //qInfo(QString::number(resourcesIndex).toStdString().c_str());
        QString resourceName = resourcesNames[resourcesIndex];
        //qInfo(resourceName.toStdString().c_str());
        QString resourceNameT = QT_TRANSLATE_NOOP("resources", resourceName);
        quint32 resourceAmount = resourcesAmount[resourcesIndex];
        addResource(qResourcesHBox, resourceNameT, resourceAmount);
    }
    addResourceCapacity(qResourcesHBox, QT_TRANSLATE_NOOP("resources", "granary"), granaryCapacity);
    addResource(qResourcesHBox, QT_TRANSLATE_NOOP("resources", resourcesNames[CROP]), resourcesAmount[CROP]);
    addResource(qResourcesHBox, QT_TRANSLATE_NOOP("resources", resourcesNames[FREE_CROP]), resourcesAmount[FREE_CROP]);

    qResourcesHBox->addStretch();
    qTabsResources->setLayout(qResourcesHBox);

    // assemble this into vertical boxes

    vbox->addWidget(qTop);
    vbox->addWidget(qTabsResources);

    if(screenView == SCREEN_VIEW_RESOURCES || screenView == SCREEN_VIEW_BUILDINGS)
    {
        // resources production and troops
        QVBoxLayout* qResourcesProductionTroopsVBox = new QVBoxLayout,
                   * qResourcesProductionVBox = new QVBoxLayout,
                   * qTroopsVBox = new QVBoxLayout;
        QHBoxLayout* qResourcesProductionTroopsHBox = new QHBoxLayout;
        QWidget * qResourcesProductionTroops = new QWidget,
                * qResourcesProductionTroopsForBox = new QWidget,
                * qResourcesProduction = new QWidget,
                * qTroops = new QWidget;

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
        //addTroopLine(qTroopsVBox, QT_TRANSLATE_NOOP("resources", "hero"), 1);
        addTroopLine(qTroopsVBox);
        qTroops->setLayout(qTroopsVBox);

        qResourcesProductionTroopsVBox->addWidget(qResourcesProduction);
        qResourcesProductionTroopsVBox->addWidget(qTroops);
        qResourcesProductionTroops->setLayout(qResourcesProductionTroopsVBox);

        qResourcesProductionTroopsHBox->addWidget(qResourcesProductionTroops);
        qResourcesProductionTroopsHBox->addStretch(2);
        qResourcesProductionTroopsForBox->setLayout(qResourcesProductionTroopsHBox); // if do like others above got a potential memory leak

        vbox->addStretch(1);
        if(screenView == SCREEN_VIEW_BUILDINGS)
        {
            //qResourcesProductionTroopsForBox->setDisabled(true);//->setVisible(false);//->hide();
            QSizePolicy spRetain = qResourcesProductionTroopsForBox->sizePolicy();
            spRetain.setRetainSizeWhenHidden(true);
            qResourcesProductionTroopsForBox->setSizePolicy(spRetain);
            qResourcesProductionTroopsForBox->hide();
        }
        vbox->addWidget(qResourcesProductionTroopsForBox);
        QWidget* qCurrentlyBuilding = getCurrentlyBuildingWidget();
        if(qCurrentlyBuilding != nullptr)
        {
            vbox->addWidget(qCurrentlyBuilding);
        }
        vbox->addStretch(2);
    }

    if(screenView == SCREEN_VIEW_MAP)
    {
        vbox->addWidget(getMapScreen());
        vbox->addStretch();
    }
    else if(screenView == SCREEN_VIEW_STATISTICS)
    {
        vbox->addWidget(getStatisticsScreen());
    }
    else if(screenView == SCREEN_VIEW_REPORTS)
    {
        vbox->addWidget(getReportsScreen());
    }
    else if(screenView == SCREEN_VIEW_RESOURCE)
    {
        vbox->addWidget(getResourceScreen());
        vbox->addStretch(2);
    }
    else if(screenView == SCREEN_VIEW_PROFILE)
    {
        vbox->addWidget(getProfileScreen());
        vbox->addStretch();
    }
    else if(screenView == SCREEN_VIEW_VILLAGE)
    {
        vbox->addWidget(getVillageScreen());
        vbox->addStretch();
    }
    else if(screenView == SCREEN_VIEW_BUILDING)
    {
        bool isImprovement = buildings[screenViewBuildingsIndex].second > 0;
        QWidget* qBuildingScreen = isImprovement ? getBuildingImprovementScreen() : getBuildingScreen(); // Test1
        vbox->addWidget(qBuildingScreen);
        if(screenViewBuildingsIndex == BUILDING_SLOT_WALL || isImprovement) // could make an exception for academy if research possibilities exceeds height
            vbox->addStretch();
    }
    else
    {
        vbox->addStretch(1);
    }

    screen->setLayout(vbox);

    myWindow->setCentralWidget(screen);
}

// how to make our protocol quantum resistant ? elliptic curve ?
// executed for name: warehouse, lumber, clay, iron, granary, crop
QWidget* getResourceInfo(QString name, quint32 capacity, QColor backgroundColor, QColor foregroundColor)
{
    QWidget* resourceCapacity = new QWidget,
           * resourceCapacityForV = new QWidget;
    QHBoxLayout* resourceCapacityHBox = new QHBoxLayout;
    QVBoxLayout* resourceCapacityVBox = new QVBoxLayout; // or maybe do a vertical and horizontal inside not the reverse

    resourceCapacityHBox->addWidget(getQLabel("resources/" + name, true, name, "resources"));
    QLabel* qLabel = new QLabel(std::to_string(capacity).c_str());
    //QLabel* qLabelBis = new QLabel(std::to_string(capacity).c_str()); // TODO: add a bar to precise ratio state
    quint8 resourceIndex = getResourceIndexFromName(name);

    if(resourceIndex != QUINT8_MAX_VALUE)
    {
        /*if(resourceIndex < 3)
        {
        }*/

    }
    //qInfo(name.toStdString().c_str());
    //qLabel->setFont(QFont("Impact", 12));

    //resourceCapacityVBox->setContentsMargins(0, 0, 0, 0);

    resourceCapacityVBox->addWidget(qLabel);
    //resourceCapacityVBox->addWidget(qLabelBis);
    resourceCapacityForV->setLayout(resourceCapacityVBox);

    resourceCapacityHBox->addWidget(resourceCapacityForV);

    resourceCapacity->setLayout(resourceCapacityHBox);
    if(backgroundColor != QColor())
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

QWidget* getResource(QString name, quint32 amount, bool needBackground)
{
    QWidget* qWidget = getResourceInfo(name, amount, needBackground ? QColor(227, 212, 188) : QColor(), Qt::black);
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
    if(name != "none")
    {
        hbox->addWidget(getQLabel("troops/" + name + ".png"));
        hbox->addWidget(new QLabel(QString::number(amount) + " " + firstUppercase(translator.translate("resources", name.toStdString().c_str()))));
    }
    else
        hbox->addWidget(new QLabel(QObject::tr("none")));
    hbox->addStretch();
    troopLine->setLayout(hbox);
    qTroopsVBox->addWidget(troopLine);
}

void updateResourcesAmount(quint32 currentTime)
{
    if(currentTime == 0)
        currentTime = getSeconds();
    double timeDelta = currentTime - timestampGameClosed,
           timeFraction = timeDelta / 3600;
    //qInfo(QString::number(timeFraction).toStdString().c_str());
    for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
    {
        quint32 resourceAmount = initialResourcesAmount[resourcesIndex] + resourcesProduction[resourcesIndex] * timeFraction;
        if(resourcesIndex < RESOURCES_NUMBER - 1)
        {
            if(resourceAmount > warehouseCapacity)
            {
                resourceAmount = warehouseCapacity;
            }
        }
        else // crop only
        {
            if(resourceAmount > granaryCapacity)
            {
                resourceAmount = granaryCapacity;
            }
        }
        resourcesAmount[resourcesIndex] = resourceAmount;
        //qInfo((QString::number(resourcesIndex) + " " + QString::number(resourceAmount)).toStdString().c_str());
    }
}

void updateScreen(/*bool refresh*/)
{
    updateResourcesAmount();
    //if(screenView != SCREEN_VIEW_STATISTICS)
    /*if(!refresh)
        setResourcesScreen();
    else*/
    {
        if(screenView != SCREEN_VIEW_BUILDING || (screenView == SCREEN_VIEW_BUILDING && lastScreenView != SCREEN_VIEW_BUILDING)) // TODO: manage this case in a better way to have resources updates etc and not resetting tab or scroll - doesn't work that way anyway
            setResourcesScreen();
        lastScreenView = screenView;
        return;
        // TODO: could at least update screen only when resources amount change etc - otherwise have tooltip problem

        QWidget* centralWidget = myWindow->centralWidget();
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
}
