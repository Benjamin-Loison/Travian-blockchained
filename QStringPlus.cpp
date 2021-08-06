#include "QStringPlus.h"

QString firstUppercase(QString s)
{
    s[0] = s[0].toTitleCase();
    return s;
}
