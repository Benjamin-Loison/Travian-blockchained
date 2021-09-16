#include <QWidget>
#include "QStringPlus.h"

QString firstUppercase(QString s)
{
    if(s == "") return s; // shouldn't happen but likewise can debug without crashing
    s[0] = s[0].toTitleCase();
    return s;
}

// need to be a macro make a ctr otherwise as a function it doesn't work

//#define cStr(s) ( s.toStdString().c_str() )
// can't achieve to make it work :'(
// if put a comment at the end of the cpp file it involves error in the header, I start hating QtCreator

const char* cStr(const QString s)
{
    return s.toStdString().c_str(); // returning address of local temporary object, should solve this with a macro cf header file...
}

