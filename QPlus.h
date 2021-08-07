#ifndef DEF_QUTIL_H
#define DEF_QUTIL_H

#include <QIcon>
#include <QPixmap>
#include <QLabel>

QIcon getQIcon(QString path);
QPixmap getQPixmap(QString path);
QLabel* getQLabel(QString path, bool needPointingHandCursor = false);

#endif
