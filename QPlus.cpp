#include "QPlus.h"

QString assetsFolder = "assets/";

QIcon getQIcon(QString path)
{
    return QIcon(assetsFolder + path);
}

QPixmap getQPixmap(QString path)
{
    return QPixmap(assetsFolder + path);
}
