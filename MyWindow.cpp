#include <QApplication> // why need this if it is also in the header ?!
#include <QtWidgets>
#include <QPainter>
#include "QStringPlus.h"
#include "QPlus.h"
#include "resources.h"
#include "main.h"
#include "MyWindow.h"

MyWindow::MyWindow()
{
    //setFixedSize(200, 100);

    screenView = SCREEN_VIEW_SELECT_TRIBE;

    setWindowIcon(getQIcon("travian.png"));
    setWindowTitle("Travian blockchained");

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
    vbox->addWidget(confirmButton);

    QWidget* screen = new QWidget;
    screen->setLayout(vbox);
    setCentralWidget(screen);

    connect(confirmButton, SIGNAL(clicked()), this, SLOT(setChooseLocationGUI()));

    addTribe(QT_TR_NOOP("gauls"), "phalanx", tr("Low time requirements"), tr("Loot protection and good defense"), tr("Excellent, fast cavalry"), tr("Well suited to new players"));
    addTribe(QT_TR_NOOP("romans"), "legionnaire", tr("Moderate time requirements"), tr("Can develop villages the fastest"), tr("Very strong but expensive troops"), tr("Hard to play for new players"));
    addTribe(QT_TR_NOOP("teutons"), "clubswinger", tr("High time requirements"), tr("Good at looting in early game"), tr("Strong, cheap infantry"), tr("For aggressive players"));
}

QLabel* MyWindow::setTitle(QString title)
{
    QLabel* qTitle = new QLabel(title);
    qTitle->setAlignment(Qt::AlignCenter);
    qTitle->setFont(QFont("Comic Sans MS", 20, QFont::Bold));
    return qTitle;
}

void MyWindow::addTribe(QString tribeName, QString troopName, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended)
{
    QWidget* tab = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout;

    QWidget* subTabs = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;

    addTribeText(vbox, troopName, timeRequirement);
    addTribeText(vbox, troopName, speciality);
    addTribeText(vbox, troopName, troopsTraining);
    addTribeText(vbox, troopName, designedForUsers);

    subTabs->setLayout(vbox);
    hbox->addWidget(subTabs);

    QLabel * qIcon = new QLabel();
    qIcon->setPixmap(getQPixmap(tribeName + ".png"));
    hbox->addWidget(qIcon);

    tab->setLayout(hbox);

    m_tabs->addTab(tab, getQIcon(tribeName + ".png"), firstUppercase(tr(tribeName.toStdString().c_str())));
}

void MyWindow::addTribeText(QVBoxLayout* vbox, QString troopName, QString text)
{
    QWidget* qLine = new QWidget;
    QLabel* qText = new QLabel(text), // it's sad that tr here doesn't work "as expected"
          * qIcon = new QLabel();
    qIcon->setPixmap(getQPixmap(troopName + ".png")); // there is maybe a way to directly make a picture but I'm learning Qt, be kind

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
    screenView = SCREEN_VIEW_SELECT_LOCATION;

    QWidget* screen = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout;
    QLabel* title = setTitle(tr("Select your starting position")), // could make a macro ^^
          * paratext = new QLabel(tr("Where do you want to start building up your empire ? Use the \"recommended\" area for the most ideal location. Or select the area where your friends are located and team up !"));
    paratext->setWordWrap(true);
    vbox->addWidget(title);
    vbox->addWidget(paratext);

    QLabel* qIcon = new QLabel();

    QPixmap map = getQPixmap("locations.png"), // map already used
            banner = getQPixmap("banner.png");

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

    painter->end(); // otherwise there is an error in logs

    qIcon->setPixmap(map);
    qIcon->setAlignment(Qt::AlignCenter);
    qIcon->setCursor(Qt::PointingHandCursor);
    //connect(qIcon, SIGNAL(clicked()), this, SLOT(chooseLocation())); // TODO: not that easy
    vbox->addWidget(qIcon);

    QPushButton* confirmButton = new QPushButton(tr("Confirm"));
    vbox->addWidget(confirmButton);
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(startGame()));
    screen->setLayout(vbox);
    setCentralWidget(screen);
}

void MyWindow::manageBackground()
{
    // this background while choosing tribe and location was initially a bug but it renders quite fine ^^
    if(screenView == SCREEN_VIEW_SELECT_TRIBE || screenView == SCREEN_VIEW_SELECT_LOCATION)
    {
        return;
    }
    QPixmap qBackgroundPixmap = getQPixmap("resourcesBackground.jpg");
    QPainter* painter = new QPainter(&qBackgroundPixmap);

    painter->drawPixmap(605, 193, getQPixmap("resources3.png"));
    painter->drawPixmap(773, 298, getQPixmap("village.png")); // should add tooltip in the future
    painter->end();

    qBackgroundPixmap = qBackgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, qBackgroundPixmap);
    setPalette(palette);
    //setAutoFillBackground(true);
}

void MyWindow::resizeEvent(QResizeEvent* evt)
{
    manageBackground();

    QMainWindow::resizeEvent(evt);
}

void MyWindow::chooseLocation()
{
    QMessageBox::information(this, "Titre de la fenêtre", "Choosed location !");
}

void MyWindow::startGame()
{
    screenView = SCREEN_VIEW_RESOURCES;
    //QMessageBox::information(this, "Titre de la fenêtre", "Bonjour et bienvenueà tous les Zéros !");

    timestampVillageStart = QDateTime::currentSecsSinceEpoch();
    timestampGameRestored = timestampVillageStart;

    setResourcesScreen(this);
    manageBackground();

    QTimer* timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(refreshLoop()));
    timer->start(1000);

}

void MyWindow::refreshLoop()
{
    setResourcesScreen(this); // let's not assume + 1 each time in case of desync etc
}
