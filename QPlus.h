#ifndef DEF_QUTIL_H
#define DEF_QUTIL_H

#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QPen>
#include <QPainter>

/*

#include <limits>

std::numeric_limits<quint32>::min();
std::numeric_limits<quint32>::max();

*/

#define QUINT8_MAX_VALUE 255
#define QUINT32_MAX_VALUE 4294967295

QIcon getQIcon(QString path);
QPixmap getQPixmap(QString path, quint16 maxSize = 0);
QLabel* getQLabel(QString path, bool needPointingHandCursor = false, QString name = "", QString context = "", quint16 maxSize = 0);
void setColor(QWidget* qWidget, QColor backgroundColor, QColor foregroundColor = QColor()),
     drawCircle(QPainter* painter, quint16 centerX, quint16 centerY, quint16 circleSize, QString text, bool shadow = false);
QString getTime(),
        call(QString command),
        getHoursMinutesSecondsStr(quint32 time);

extern QString assetsFolder;
int distance(int x0, int y0, int x1, int y1);
bool isInEllipse(int h, int k, int x, int y, int rX, int rY);
QFrame* getHorizontalLine();

#endif
