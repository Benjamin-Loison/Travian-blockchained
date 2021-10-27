#ifndef DEF_CRYPTOPLUS_H
#define DEF_CRYPTOPLUS_H

#include <QString>

extern QString PRIVATE_KEY,
               PUBLIC_KEY;

QString doubleSHA512(QString input),
        callOpenSSL(QString arguments);

#endif
