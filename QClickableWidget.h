#ifndef DEF_QCLICKABLEWIDGET_H
#define DEF_QCLICKABLEWIDGET_H

#include "QPlus.h"
#include <QWidget>
#include <QMouseEvent> // used to be able to compile without this if don't call x and y function
#include "resources.h"
#include "buildings.h"
#include "QPlus.h"
#include "main.h"

class QClickableWidget : public QWidget
{
    Q_OBJECT

    public:
        QClickableWidget(QWidget* parent = nullptr) : QWidget(parent)
        {

        }

    protected:
        void mousePressEvent(QMouseEvent* event) override
        {
            int x = event->x(), // imagine compiler saying deprecated but not the doc, just imagine - wait my compiler invits me to use position which doesn't exist u_u
                y = event->y();
            QSize screenSize = myWindow->size(); // if named window it's a problem here
            if(screenView == SCREEN_VIEW_RESOURCES)
            {
                //qInfo((QString::number(x) + " " + QString::number(y)).toStdString().c_str());
                //qInfo((QString::number(screenSize.width()) + " " + QString::number(screenSize.height())).toStdString().c_str());
                int rX = x * 1921 / screenSize.width(), // size of resourcesBackground.jpg
                    rY = y * 862 / screenSize.height(); // QSize includes such functions
                for(quint8 farmsIndex = 0; farmsIndex < FARMS_NUMBER; farmsIndex++)
                {
                    int farmX = farmsScreen[farmsIndex][0],
                        farmY = farmsScreen[farmsIndex][1];
                    if(distance(farmX, farmY, rX, rY) <= CIRCLE_SIZE) // could be cool in the future to have cursor clickable on farms
                    {
                        //qInfo(QString::number(farmsIndex).toStdString().c_str()); // works perfectly :)
                        setResourceFarmScreen(farmsIndex);
                    }
                }
            }
            else if(screenView == SCREEN_VIEW_BUILDINGS)
            {
                int rX = x * 2541 / screenSize.width(),
                    rY = y * 1282 / screenSize.height();
                for(quint8 buildingsIndex = 0; buildingsIndex < BUILDINGS_SLOTS_NUMBER; buildingsIndex++)
                {
                    int buildingX = buildingsScreen[buildingsIndex][0],
                        buildingY = buildingsScreen[buildingsIndex][1];
                    if(isInEllipse(buildingX, buildingY, rX, rY, CIRCLE_SIZE * 2, CIRCLE_SIZE))
                    {
                        //qInfo(QString::number(buildingsIndex).toStdString().c_str());
                        setBuildingScreen(buildingsIndex);
                    }
                }
            }
        }

};

#endif
