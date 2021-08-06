#include <QApplication>
#include <QtWidgets>
#include "MyWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("travianblockchained_fr");
    app.installTranslator(&translator);

    MyWindow window;
    window.show();

    return app.exec();
}
