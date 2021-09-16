#include "village.h"
#include "resources.h"
#include "main.h"
#include "QStringPlus.h"
#include "QPlus.h"
#include <QHBoxLayout>
#include <QLabel>

void sendTroopsGUI()
{

}

void sendMerchants()
{

}

QWidget* getVillageScreen()
{
    QWidget* screen = new QWidget,
           * village = new QWidget,
           * map = new QWidget,
           * rightColumnMap = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout,
               * mapHBox = new QHBoxLayout;
    QVBoxLayout* vbox = new QVBoxLayout,
               * rightColumnMapVBox = new QVBoxLayout;
    vbox->addWidget(new QLabel(firstUppercase(QObject::tr("%1's village").arg(nickname)) + " <font color=\"grey\">(" + QObject::tr("Capital") + ")</font> " + getCoordinatesStr()));

    QPixmap background = getQPixmap("village/villageResources3.png");
    QPainter* painter = new QPainter(&background);
    painter->drawPixmap(116, 85, getQPixmap("village/villageVillage.png"));
    painter->end();
    QLabel* mapLabel = new QLabel();
    mapLabel->setPixmap(background);

    mapHBox->addWidget(mapLabel); // getQLabel("village/villageResources3.png")
    rightColumnMapVBox->addWidget(new QLabel(QObject::tr("Tribe") + " <b>" + translator.translate("tribes", getTribe().toStdString().c_str()) + "</b>"));
    rightColumnMapVBox->addWidget(new QLabel(QObject::tr("Owner") + " <b>" + nickname + "</b>"));
    rightColumnMapVBox->addWidget(new QLabel(QObject::tr("Population") + " <b>" + QString::number(population) + "</b>"));
    rightColumnMapVBox->addWidget(new QLabel(QObject::tr("Distance") + " <b>" + QString::number(0) + " " + QObject::tr("fields") + "</b>"));
    rightColumnMap->setLayout(rightColumnMapVBox);
    mapHBox->addWidget(rightColumnMap);
    map->setLayout(mapHBox);
    vbox->addWidget(map);

    QPushButton* sendTroopsButton = new QPushButton(QObject::tr("Send troops")),
               * sendMerchantsButton = new QPushButton(QObject::tr("Send merchant(s)"));
    QObject::connect(sendTroopsButton, &QPushButton::clicked, [](){ sendTroopsGUI(); });

    QObject::connect(sendMerchantsButton, &QPushButton::clicked, [](){ sendMerchants(); });

    vbox->addWidget(sendTroopsButton);
    vbox->addWidget(sendMerchantsButton);
    village->setLayout(vbox);
    hbox->addStretch();
    setColor(village, Qt::white);
    hbox->addWidget(village);
    hbox->addStretch();
    screen->setLayout(hbox);
    return screen;
}
