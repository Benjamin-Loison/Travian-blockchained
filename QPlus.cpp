#include "QStringPlus.h"
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

QLabel* getQLabel(QString path, bool needPointingHandCursor)
{
    QLabel* qLabel = new QLabel;
    if(needPointingHandCursor)
    {
        qLabel->setCursor(Qt::PointingHandCursor);
        qLabel->setAlignment(Qt::AlignTop);
        qLabel->setToolTip(QObject::tr(firstUppercase(path).toStdString().c_str()));
        path += ".png";
    }
    qLabel->setPixmap(getQPixmap(path));
    return qLabel;
}
