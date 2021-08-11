#include "QStringPlus.h"
#include "main.h"
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

QLabel* getQLabel(QString path, bool needPointingHandCursor, QString name, QString context)
{
    QLabel* qLabel = new QLabel;
    if(needPointingHandCursor)
    {
        qLabel->setCursor(Qt::PointingHandCursor);
        //qLabel->setAlignment(Qt::AlignTop);
        if(name == "")
            name = path;
        qLabel->setToolTip(firstUppercase(context != "" ? translator.translate(context.toStdString().c_str(), name.toStdString().c_str()) : QObject::tr(name.toStdString().c_str())));
        path += ".png";
    }
    qLabel->setPixmap(getQPixmap(path));
    return qLabel;
}

void setColor(QWidget* qWidget, QColor backgroundColor, QColor foregroundColor)
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, backgroundColor);
    qWidget->setAutoFillBackground(true);
    if(foregroundColor != QColor())
    {
        pal.setColor(qWidget->foregroundRole(), foregroundColor);
    }
    qWidget->setPalette(pal);

    /*QPalette palette = qWidget->palette();
    palette.setColor(qWidget->backgroundRole(), backgroundColor);
    if(foregroundColor != QColor())
        palette.setColor(qWidget->foregroundRole(), foregroundColor);
    qWidget->setPalette(palette);*/
}
