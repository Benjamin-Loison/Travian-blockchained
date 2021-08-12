#include <QApplication>
#include <QtWidgets>
#include "main.h"

screenViewEnum screenView;
tribeEnum tribe;
QTranslator translator;
MyWindow* window;

// instead of linking README.md etc to be able to commit and push without forcing if added content directly through web interface, could just use a .gitignore no ? if so upload tools

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    translator.load("travianblockchained_fr");
    app.installTranslator(&translator);

    // the "Ready to rule the world ?" GUI won't be designed because it's useless (at least I consider it as useless)

    window = new MyWindow();
    window->show();

    return app.exec();
}

QString getTribe(tribeEnum tribe)
{
    switch(tribe)
    {
        case TRIBE_GAULS:
            return "gauls";
        case TRIBE_ROMANS:
            return "romans";
        case TRIBE_TEUTONS:
            return "teutons";
    }
}

QString getTribe()
{
    return getTribe(tribe);
}
