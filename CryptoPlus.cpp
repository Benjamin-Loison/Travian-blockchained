#include <QCryptographicHash>
#include <QProcess>
#include "CryptoPlus.h"
#include "QPlus.h"
#include "main.h"

QString PRIVATE_KEY = USER_FOLDER + "private-key.pem",
        PUBLIC_KEY = USER_FOLDER + "public-key.pem";

QString SHA512(QString input)
{
    // working with 6.1.1 on my Windows
    QCryptographicHash cryptographicHash(QCryptographicHash::Sha512); // can't do classical " = QCryptographicHash" on Linux... (Qt < 6.1.1) (use of deleted function QCryptographicHash::QCryptographicHash(const QCryptographicHash&) with Qt 5.12.8)
    QByteArray byteArray = input.toUtf8(),
               hashedByteArray = cryptographicHash.hash(byteArray, QCryptographicHash::Sha512); // why have to repeat it ?!
    QString hashed = hashedByteArray.toHex();
    return hashed;
}

QString doubleSHA512(QString input)
{
    QString firstOutput = SHA512(input),
            secondOutput = SHA512(firstOutput);
    return secondOutput;
}

QString callOpenSSL(QString arguments)
{
    return call("openssl " + arguments);
}

// P-256 is maybe unsafe use instead https://fr.wikipedia.org/wiki/Curve25519 according to Bruce Schneier

