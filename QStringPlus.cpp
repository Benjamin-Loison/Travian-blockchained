#include "QStringPlus.h"

// this should be in Qt by default u_u
QString firstUppercase(QString s)
{
    s[0] = s[0].toTitleCase();
    return s;
}

const char* cStr(const QString s)
{
    return s.toStdString().c_str();
}
