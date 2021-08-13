#include <QCryptographicHash>
#include "CryptoPlus.h"

QString SHA256(QString input)
{
    QCryptographicHash cryptographicHash = QCryptographicHash(QCryptographicHash::Sha256);
    QByteArray byteArray = input.toUtf8(),
               hashedByteArray = cryptographicHash.hash(byteArray, QCryptographicHash::Sha256); // why have to repeat it ?!
    QString hashed = hashedByteArray.toHex();
    //qInfo(hashed.toStdString().c_str()); // it works
    return hashed;
}

QString doubleSHA256(QString input)
{
    QString firstOutput = SHA256(input),
            secondOutput = SHA256(firstOutput);
    return secondOutput;
}

// AES
// crypto++ https://stackoverflow.com/a/21885471/7123660 ?
// or https://github.com/KDE/qca ?
// https://github.com/bricke/Qt-AES
//

// RSA
//

// SEEMS TO BE THE SOLUTION FOR BOTH
// https://github.com/QuasarApp/Qt-Secret
//
