#include "QStringPlus.h"
#include "main.h"
#include "QPlus.h"
using namespace std;

QString assetsFolder = "assets/";

QIcon getQIcon(QString path)
{
    return QIcon(assetsFolder + path);
}

QPixmap getQPixmap(QString path, quint16 maxSize)
{
    QPixmap qPixmap = QPixmap(assetsFolder + path);
    if(maxSize != 0)
    {
        //qInfo(("redim " + QString::number(maxSize)).toStdString().c_str());
        qPixmap = qPixmap.scaled(maxSize, maxSize, Qt::KeepAspectRatio);
    }
    return qPixmap;
}

QLabel* getQLabel(QString path, bool needPointingHandCursor, QString name, QString context, quint16 maxSize)
{
    QLabel* qLabel = new QLabel;
    if(needPointingHandCursor)
    {
        qLabel->setCursor(Qt::PointingHandCursor);
        if(name == "")
            name = path;
        qLabel->setToolTip(firstUppercase(context != "" ? translator.translate(context.toStdString().c_str(), name.toStdString().c_str()) : QObject::tr(name.toStdString().c_str())));
        path += ".png";
    }
    qLabel->setPixmap(getQPixmap(path, maxSize));
    return qLabel;
}

void setColor(QWidget* qWidget, QColor backgroundColor, QColor foregroundColor)
{
    QPalette pal = QPalette();
    if(backgroundColor != QColor()) // and what do we do if we want black background ?
        pal.setColor(QPalette::Window, backgroundColor);
    qWidget->setAutoFillBackground(true);
    if(foregroundColor != QColor())
    {
        pal.setColor(qWidget->foregroundRole(), foregroundColor);
    }
    qWidget->setPalette(pal);
}

void drawCircle(QPainter* painter, quint16 centerX, quint16 centerY, quint16 circleSize, QString text, bool shadow)
{
    if(text != "0")
        shadow = false;
    quint16 circleSizeDiv2 = circleSize / 2;
    centerX -= circleSizeDiv2;
    centerY -= circleSizeDiv2;
    QRectF greenRect(centerX, centerY, shadow ? 2 * circleSize : circleSize, circleSize);
    QColor buildingGreen = shadow ? QColor(122, 131, 32) : QColor(192, 226, 72);
    QPen defaultPen = painter->pen();
    if(!shadow)
    {
        QPen greenPen = QPen(buildingGreen);
        greenPen.setWidth(3);
        painter->setBrush(Qt::white);
        painter->setPen(greenPen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(buildingGreen);
    }
    painter->drawEllipse(greenRect);

    painter->setPen(defaultPen); // if put this statement in the if, it doesn't restore the pen required for buildings built
    if(text != "0")
    {
        QSize textSize = QFontMetrics(painter->font()).size(Qt::TextSingleLine, text);
        unsigned short textWidthDiv2 = textSize.width(),
                       textHeightDiv2 = textSize.height();
        painter->drawText(centerX + textWidthDiv2 * (text.length() == 2 ? 0.5 : 1.4), centerY + textHeightDiv2, text); // why this factor ?
    }
}

QString getTime()
{
    return QTime::currentTime().toString("hh:mm:ss,zzz");
}

QString call(QString command)
{
    QProcess process;
    QStringList arguments = command.split(" ");
    QString program = arguments[0];
    arguments.takeFirst();
    process.start(program, arguments);
    process.waitForFinished(-1);

    QString output(process.readAllStandardOutput()); // can't be QString output = process.readAllStandardOutput(); :'(
    //qInfo(output.toStdString().c_str());
    //QString error(process.readAllStandardError());
    //qInfo(error.toStdString().c_str());
    return output;
}

int distance(int x0, int y0, int x1, int y1) // why working with int ?
{
    return qSqrt(qPow(x0 - x1, 2) + qPow(y0 - y1, 2));
}

// https://math.stackexchange.com/a/76463
bool isInEllipse(int h, int k, int x, int y, int rX, int rY)
{
    return (qPow(x - h, 2) / qPow(rX, 2) + qPow(y - k, 2) / qPow(rY, 2)) <= 1;
}

tuple<quint8, quint8, quint8> getSecondsMinutesHours(quint32 time)
{
    quint8 seconds = time % 60,
           minutes = ((time - seconds) / 60) % 60,
           hours = (time - seconds - minutes * 60) / 3600;
    tuple<quint8, quint8, quint8> tupl = make_tuple(seconds, minutes, hours);
    return tupl;
}

QString getHoursMinutesSecondsStr(quint32 time)
{
    tuple<quint8, quint8, quint8> secondsMinutesHours = getSecondsMinutesHours(time);
    quint8 seconds = get<0>(secondsMinutesHours),
           minutes = get<1>(secondsMinutesHours),
           hours = get<2>(secondsMinutesHours);
    QString secondsStr = QString::number(seconds),
            minutesStr = QString::number(minutes),
            hoursStr = QString::number(hours);
    if(secondsStr.length() == 1)
        secondsStr = "0" + secondsStr;
    if(minutesStr.length() == 1)
        minutesStr = "0" + minutesStr;
    return hoursStr + ":" + minutesStr + ":" + secondsStr;
}

QFrame* getHorizontalLine()
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);
    return line;
}
