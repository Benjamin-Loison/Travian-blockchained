#ifndef DEF_QCLICKABLELABEL_H
#define DEF_QCLICKABLELABEL_H

#include "QPlus.h"
#include <QWidget>
#include <QMouseEvent>
#include "main.h"

// copied from QClickableWidget
class QClickableLabel : public QLabel
{
    Q_OBJECT

    public:
        QClickableLabel(QWidget* parent = nullptr) : QLabel(parent)
        {

        }

    protected:
        void mousePressEvent(QMouseEvent* event) override
        {
            /*int*/double x = event->x(),
                          y = event->y();
            QSize screenSize = QSize(317, 297);//myWindow->size();
            if(screenView == SCREEN_VIEW_SELECT_LOCATION)
            {
                double rX = x / screenSize.width(),
                       rY = y / screenSize.height();
                bool isNorth = rY <= 0.5,
                     isWest = rX <= 0.5;
                //qInfo((QString::number(isNorth) + " " + QString::number(isWest)).toStdString().c_str());
                //qInfo((QString::number(rY) + " " + QString::number(rX)).toStdString().c_str());
                myWindow->setChooseLocationGUI(false, isNorth, isWest);
            }
        }

};

#endif
