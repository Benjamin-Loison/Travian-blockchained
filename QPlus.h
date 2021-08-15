#ifndef DEF_QUTIL_H
#define DEF_QUTIL_H

#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QPen>
#include <QPainter>

QIcon getQIcon(QString path);
QPixmap getQPixmap(QString path);
QLabel* getQLabel(QString path, bool needPointingHandCursor = false, QString name = "", QString context = "");
void setColor(QWidget* qWidget, QColor backgroundColor, QColor foregroundColor = QColor()),
     drawCircle(QPainter* painter, quint16 centerX, quint16 centerY, quint16 circleSize, QString text);
QString getTime();

#endif
