#include <QApplication> // why need this if it is also in the header ?!
#include <QtWidgets>
#include <QPainter>
#include "QStringPlus.h"
#include "QPlus.h"
#include "resources.h"
#include "buildings.h"
#include "troops.h"
#include "main.h"
#include "MyWindow.h"
#include "CryptoPlus.h"
#include "QClickableWidget.h"

MyWindow::MyWindow()
{
    //setFixedSize(200, 100);

    setWindowIcon(getQIcon("travian.png"));
    setWindowTitle("Travian blockchained");
}

void MyWindow::setChooseTribeGUI()
{
    screenView = SCREEN_VIEW_SELECT_TRIBE;

    QVBoxLayout* vbox = new QVBoxLayout;
    QLabel* title = setTitle(tr("Select your tribe")),
          * paratext = new QLabel(tr("Great empires begin with important decisions ! Are you an attacker who loves competition ? Or is your time investment rather low ? Are you a team player who enjoys building up a thriving economy to forge the anvil ?"));
    paratext->setWordWrap(true);

    m_tabs = new QTabWidget();
    m_tabs->setGeometry(30, 20, 240, 160);

    QPushButton* confirmButton = new QPushButton(tr("Confirm"));

    vbox->addWidget(title);
    vbox->addWidget(paratext);
    vbox->addWidget(m_tabs);

    QWidget* nicknameSelection = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel(tr("Nickname:")));
    nicknameLineEdit = new QLineEdit("");
    hbox->addWidget(nicknameLineEdit);
    hbox->addStretch();
    nicknameSelection->setLayout(hbox);
    vbox->addWidget(nicknameSelection);

    vbox->addWidget(confirmButton);

    QWidget* screen = new QWidget;
    screen->setLayout(vbox);
    setCentralWidget(screen);

    connect(confirmButton, SIGNAL(clicked()), this, SLOT(setChooseLocationGUI()));

    addTribe(TRIBE_GAULS, tr("Low time requirements"), tr("Loot protection and good defense"), tr("Excellent, fast cavalry"), tr("Well suited to new players"));
    addTribe(TRIBE_ROMANS, tr("Moderate time requirements"), tr("Can develop villages the fastest"), tr("Very strong but expensive troops"), tr("Hard to play for new players"));
    addTribe(TRIBE_TEUTONS, tr("High time requirements"), tr("Good at looting in early game"), tr("Strong, cheap infantry"), tr("For aggressive players"));
}

void MyWindow::keyPressEvent(QKeyEvent* pe)
{
    if(pe->key() == Qt::Key_Return)
    {
        if(screenView == SCREEN_VIEW_SELECT_TRIBE)
            setChooseLocationGUI();
        else if(screenView == SCREEN_VIEW_SELECT_LOCATION)
            startGame();
    }
}

QLabel* MyWindow::setTitle(QString title)
{
    QLabel* qTitle = new QLabel(title);
    qTitle->setAlignment(Qt::AlignCenter);
    qTitle->setFont(QFont("Comic Sans MS", 20, QFont::Bold));
    return qTitle;
}

void MyWindow::addTribe(tribeEnum tribe, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended)
{
    Q_UNUSED(recommended)

    QWidget* tab = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout;

    QWidget* subTabs = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;

    QString tribeName = getTribe(tribe),
            troopName = troopsNames[tribe][0];
    addTribeText(vbox, tribeName, troopName, timeRequirement);
    addTribeText(vbox, tribeName, troopName, speciality);
    addTribeText(vbox, tribeName, troopName, troopsTraining);
    addTribeText(vbox, tribeName, troopName, designedForUsers);

    subTabs->setLayout(vbox);
    hbox->addWidget(subTabs);

    QLabel* qIcon = new QLabel(); // could use getQLabel instead ?
    qIcon->setPixmap(getQPixmap("tribes/" + tribeName + ".png"));
    hbox->addWidget(qIcon);

    tab->setLayout(hbox);

    m_tabs->addTab(tab, getQIcon(tribeName + ".png"), firstUppercase(tr(tribeName.toStdString().c_str())));
}

void MyWindow::addTribeText(QVBoxLayout* vbox, QString tribeName, QString troopName, QString text)
{
    QWidget* qLine = new QWidget;
    QLabel* qText = new QLabel(text),
          * qIcon = new QLabel();
    qIcon->setPixmap(getQPixmap("troops/" + tribeName + "/" + troopName + ".png"));

    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(qIcon);
    hbox->addWidget(qText, Qt::AlignLeft);
    qLine->setLayout(hbox);

    vbox->addWidget(qLine);
}

void drawTextCentered(QPainter* painter, unsigned short x, unsigned short y, QString text, bool withBackground = false)
{
    QSize textSize = QFontMetrics(painter->font()).size(Qt::TextSingleLine, text);
    unsigned short textWidthDiv2 = textSize.width() / 2,
                   textHeight = textSize.height();
    if(withBackground)
    {
        y += textHeight;
        painter->fillRect(x - textWidthDiv2 - 1, y - textHeight / 1.5 - 1, textWidthDiv2 * 2 + 2, textHeight + 2, QBrush(Qt::darkGreen));
    }
    painter->drawText(x - textWidthDiv2, y, text);
}

void MyWindow::setChooseLocationGUI()
{
    QString nicknameTmp = nicknameLineEdit->text();
    // should check if nickname already used
    if(nicknameTmp == "") // could check for ascii ? or let's accept UTF8 ? - just ban emots ?
    {
        QMessageBox::warning(this, tr("Invalid nickname"), tr("Your nickname can't be empty"));
        return;
    }
    nickname = nicknameTmp;

    screenView = SCREEN_VIEW_SELECT_LOCATION;

    QWidget* oldScreen = centralWidget();
    QLayout* layout = oldScreen->layout();
    QLayoutItem* tabsItem = layout->itemAt(2);
    QTabWidget* tabs = (QTabWidget*)tabsItem->widget(); // maybe the thing missing in refreshLoop is casting ?
    quint8 tabsIndex = tabs->currentIndex();
    switch(tabsIndex)
    {
        case 0:
            tribe = TRIBE_GAULS;
            break;
        case 1:
            tribe = TRIBE_ROMANS;
            break;
        case 2:
            tribe = TRIBE_TEUTONS;
            break;
    }

    // QTabWidget 2 layout

    QWidget* screen = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout;
    QLabel* title = setTitle(tr("Select your starting position")), // could make a macro ^^
          * paratext = new QLabel(tr("Where do you want to start building up your empire ? Use the \"recommended\" area for the most ideal location. Or select the area where your friends are located and team up !"));
    paratext->setWordWrap(true);
    vbox->addWidget(title);
    vbox->addWidget(paratext);

    QLabel* qIcon = new QLabel();

    QString locationsFolder = "locations/";
    QPixmap map = getQPixmap(locationsFolder + "locations.png"),
            banner = getQPixmap(locationsFolder + "banner.png");

    QPixmap* mapPtr = &map; // don't need any free ? ^^'
    QPainter* painter = new QPainter(mapPtr);

    quint16 west = 80, // could automatize but not very important because not about to change assets ^^ - should disassembly map in order to get the margin size
            east = 218,
            north = 59,
            south = 187,
            x = west, y = north,
            bannerWidthDiv2 = banner.width() / 2, bannerHeight = banner.height();

    painter->drawPixmap(x, y, banner);

    QFont bold("Verdana", 10);
    bold.setBold(true);
    painter->setFont(bold);

    drawTextCentered(painter, west + bannerWidthDiv2, north - 5, tr("NORTH-WEST"));
    drawTextCentered(painter, east + bannerWidthDiv2, north - 5, tr("NORTH-EAST"));
    drawTextCentered(painter, east + bannerWidthDiv2, south - 5, tr("SOUTH-EAST"));
    drawTextCentered(painter, west + bannerWidthDiv2, south - 5, tr("SOUTH-WEST"));

    drawTextCentered(painter, x + 9, y + bannerHeight + 5,  tr("RECOMMENDED"), true);

    painter->end();

    qIcon->setPixmap(map);
    qIcon->setAlignment(Qt::AlignCenter);
    qIcon->setCursor(Qt::PointingHandCursor);
    //connect(qIcon, SIGNAL(clicked()), this, SLOT(chooseLocation())); // TODO: not that easy - with an icon we can get where the guy clicked it seems
    vbox->addWidget(qIcon);

    QPushButton* confirmButton = new QPushButton(tr("Confirm"));
    vbox->addWidget(confirmButton);
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(startGame()));
    screen->setLayout(vbox);
    setCentralWidget(screen);
}

void drawBuilding(QPainter* painter, QString building, quint16 x, quint16 y, quint8 level, qint16 offsetX = 0, qint16 offsetY = 0)
{
    QString buildingsAssets = "buildings/" + getTribe() + "/";
    QPixmap buildingPixmap = getQPixmap(buildingsAssets + building + ".png");
    QSize buildingSize = buildingPixmap.size();
    painter->drawPixmap(x, y, buildingPixmap);
    quint16 finalX = x + buildingSize.width() / 2 + offsetX,
            finalY = y + buildingSize.height() / 2 + offsetY;
    //qInfo((building + " " + QString::number(finalX) + " " + QString::number(finalY)).toStdString().c_str());
    drawCircle(painter, finalX, finalY, CIRCLE_SIZE, QString::number(level));
}

void displayBuilding(QPainter* painter, quint8 buildingsIndex)
{
    //if(buildingsIndex == 7 || buildingsIndex == 9)
    //    return;
    QPair<quint8, quint8> building = buildings[buildingsIndex];
    quint8 buildingLevel = building.second,
           buildingType = building.first;
    bool isABuildingAWall = isAWall(buildingType);
    //qInfo(QString::number(isABuildingAWall).toStdString().c_str());
    qint16 offsetX = isABuildingAWall ? -400 : -58,
           offsetY = isABuildingAWall ? -480 : -85;
    quint16 x = buildingsScreen[buildingsIndex][0],
            y = buildingsScreen[buildingsIndex][1];
    if(buildingLevel > 0)
    {
        QString buildingName = buildingsNames[buildingType];
        drawBuilding(painter, buildingName, x + offsetX, y + offsetY, buildingLevel, isABuildingAWall ? 3 : 0, isABuildingAWall ? 210 : 0); // 120 is the image size (width and height) - perfect values
    }
    else
        drawCircle(painter, x, y, CIRCLE_SIZE, QString::number(buildingLevel), true);
}

void MyWindow::manageBackground()
{
    if(screenView == SCREEN_VIEW_SELECT_TRIBE || screenView == SCREEN_VIEW_SELECT_LOCATION)
    {
        return;
    }
    QString villageAssets = "village/",
            backgroundPath = villageAssets + (screenView == SCREEN_VIEW_RESOURCES || screenView == SCREEN_VIEW_RESOURCE ? "resources" : "buildings") + "Background.jpg";
    QPixmap qBackgroundPixmap = getQPixmap(backgroundPath);
    //QClickablePixmap* qBackgroundPixmap = new QClickablePixmap(backgroundPath);

    QPainter* painter = new QPainter(&qBackgroundPixmap);
    if(screenView == SCREEN_VIEW_RESOURCES)
    {
        painter->drawPixmap(605, 193, getQPixmap(villageAssets + "resources3.png"));
        painter->drawPixmap(773, 298, getQPixmap(villageAssets + "village.png")); // should add tooltip in the future

        for(quint8 farmsIndex = 0; farmsIndex < FARMS_NUMBER; farmsIndex++)
        {
            drawCircle(painter, farmsScreen[farmsIndex][0], farmsScreen[farmsIndex][1], CIRCLE_SIZE, QString::number(farms[farmsIndex]));
        }
        // might have troubles later because of different interfaces (on default one up crops circle is just not at the right place u_u)
    }
    else if(screenView == SCREEN_VIEW_BUILDINGS)
    {
        //drawBuilding(painter, "main building", 1070, 250, 1);
        //drawBuilding(painter, "rally point", 1350, 280, 1);
        displayBuilding(painter, BUILDINGS_SLOTS_NUMBER - 1); // need to use this code schema otherwise palisade is rendered above buildings
        for(quint8 buildingsIndex = 0; buildingsIndex < BUILDINGS_SLOTS_NUMBER - 1; buildingsIndex++)
        {
            displayBuilding(painter, buildingsIndex);
        }
    }
    painter->end();

    qBackgroundPixmap = qBackgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, qBackgroundPixmap);
    setPalette(palette);
}

void MyWindow::resizeEvent(QResizeEvent* evt)
{
    manageBackground();

    QMainWindow::resizeEvent(evt);
}

void MyWindow::startGame(bool isRestoring)
{
    screenView = SCREEN_VIEW_RESOURCES;

    if(isRestoring)
    {
        //timestampGameRestored = getSeconds();
    }
    else
    {
        QDir().mkdir(USER_FOLDER);
        QString generatePrivateKeyCommand = "ecparam -genkey -name prime256v1 -noout -out " + PRIVATE_KEY;
        callOpenSSL(generatePrivateKeyCommand);
        callOpenSSL("ec -in " + PRIVATE_KEY + " -pubout -out " + PUBLIC_KEY);

        timestampVillageStart = getSeconds();
        timestampGameClosed = timestampVillageStart;

        // do C++ gives us that default array has null values ? - it seems
        farms[0] = 2;
        farms[1] = 2;
        farms[4] = 1; // count compute each time required current production by farms levels but "that would be heavy"

        for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
        {
            initialResourcesAmount[resourcesIndex] = 750;
        }
        resourcesAmount[FREE_CROP] = 20;
        population = 8;
        culturePoints = 500; // better to manage right now even if useless likewise can do update and it works without restart
    }

    setResourceScreen();
    manageBackground();

    //refreshLoop();
    QTimer* timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(refreshLoop(/*true*/)));
    timer->start(1000);

}

void MyWindow::refreshLoop(/*bool refresh*/)
{
    updateScreen(/*refresh*/);
    //setResourcesScreen(); // let's not assume + 1 each time in case of desync etc
}
