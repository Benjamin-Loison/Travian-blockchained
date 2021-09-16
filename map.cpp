#include "map.h"
#include "main.h"
#include <QPlus.h>

void setMapScreen()
{
    screenView = SCREEN_VIEW_MAP;
}

QPixmap getMapAsset(QString file)
{
    return getQPixmap("map/" + file + ".jpg");
}

QPixmap getVillage(tribeEnum tribe)
{
    return getMapAsset(getTribe(tribe));
}

QWidget* getMapScreen()
{
    //QSize windowSize = myWindow->size(); // size, sizeHint, sizeIncrement, maximumSize
    //QRect geometry = myWindow->geometry();
    //windowSize = geometry.size();
    quint8 tileSize = 60;

    quint16 height = 600,//windowSize.height(),
            width = 600,//windowSize.width(),
            minimalDimension = qMin(height, width);
    //qInfo((QString::number(height) + " " + QString::number(width) + ": " + QString::number(minimalDimension)).toStdString().c_str());
    //qInfo(QString::number(minimalDimension).toStdString().c_str());
    quint16 backgroundSize = tileSize * /*9*/qFloor(minimalDimension / tileSize) - 1;
    QWidget* screen = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    QPixmap gaulsVillage = getVillage(TRIBE_GAULS),
            romansVillage = getVillage(TRIBE_ROMANS),
            teutonsVillage = getVillage(TRIBE_TEUTONS),
            defaultTile = getMapAsset("default"),
            background = QPixmap(backgroundSize, backgroundSize);//getMapAsset("background");

    QPainter* painter = new QPainter(&background);
    for(quint16 y = 0; y < height; y += tileSize)
    {
        for(quint16 x = 0; x < width; x += tileSize)
        {
            painter->drawPixmap(x, y, defaultTile);
        }
    }
    painter->end();
    QLabel* map = new QLabel;
    map->setPixmap(background);
    hbox->addStretch();
    hbox->addWidget(map);
    hbox->addStretch();
    screen->setLayout(hbox);
    return screen;
}
