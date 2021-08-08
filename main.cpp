#include <QApplication>
#include <QtWidgets>
#include "MyWindow.h"
#include "main.h"

screenViewEnum screenView;
QTranslator translator;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    translator.load("travianblockchained_fr");
    app.installTranslator(&translator);

    // the "Ready to rule the world ?" GUI won't be designed because it's useless (at least I consider it as useless)

    MyWindow window;
    window.show();

    return app.exec();
}
