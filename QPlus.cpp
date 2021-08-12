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

void drawCircle(QPainter* painter, quint16 centerX, quint16 centerY, quint16 circleSize, QString text)
{
    quint16 circleSizeDiv2 = circleSize / 2;
    centerX -= circleSizeDiv2;
    centerY -= circleSizeDiv2;
    QRectF greenRect(centerX, centerY, circleSize, circleSize);
    QColor buildingGreen = QColor(192, 226, 72);
    QPen greenPen = QPen(buildingGreen),
         defaultPen = painter->pen();
    greenPen.setWidth(3);
    painter->setBrush(Qt::white);
    painter->setPen(greenPen);
    painter->drawEllipse(greenRect);

    if(text != "0")
    {
        painter->setPen(defaultPen);
        QSize textSize = QFontMetrics(painter->font()).size(Qt::TextSingleLine, text);
        unsigned short textWidthDiv2 = textSize.width(),
                       textHeightDiv2 = textSize.height();
        painter->drawText(centerX + textWidthDiv2 * 1.4, centerY + textHeightDiv2, text); // why this factor ?
    }
}
