#include <QTime>
#include <QRandomGenerator>
#include "QRandomPlus.h"

QRandomGenerator qRandomGenerator;
QChar hexs[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void initializeRandom()
{
    // because we also share timestamp do we have same randomness on different computers even if seed with same time ?
    qRandomGenerator = QRandomGenerator(QTime::currentTime().msec()); // doesn't seem necessary in Among Us decentralized for instance
}

QString randomHex(quint16 length)
{
    QString res = "";
    for(quint16 i = 0; i < length / 4/* / 64*/; i++)
    {
        //quint64 randomNb = qRandomGenerator.generate64();
        quint32 randomNb = qRandomGenerator.generate() % 16;
        //res += QString(QByteArray::fromHex(QString::number(randomNb).toUtf8()));
        QChar hex = hexs[randomNb];
        res.append(hex);
    }
    return res;
}

