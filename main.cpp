#include <QApplication>
#include <QtWidgets>
#include <QSettings>
#include <QFile>
#include "resources.h"
#include "main.h"

screenViewEnum screenView;
tribeEnum tribe;
QTranslator translator;
MyWindow* window;
QString nickname;

// instead of linking README.md etc to be able to commit and push without forcing if added content directly through web interface, could just use a .gitignore no ? if so upload tools

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString languageFile = "travianblockchained_fr";
    if(translator.load(languageFile))
        app.installTranslator(&translator);
    else
        qInfo("languageFile couldn't be loaded !"); // how to inject languageFile value ?

    // the "Ready to rule the world ?" GUI won't be designed because it's useless (at least I consider it as useless)

    window = new MyWindow();
    QString settingsFile = "settings.ini";
    if(QFile::exists(settingsFile))
    {
        QSettings settings(settingsFile, QSettings::IniFormat);
        //screenView = static_cast<enum screenViewEnum>(settings.value("screenView").toInt()); // opening exactly where the user was, isn't that much interesting most of time
        tribe = static_cast<enum tribeEnum>(settings.value("tribe").toInt());
        nickname = settings.value("nickname").toString();
        timestampVillageStart = settings.value("timestampVillageStart").toUInt();
    }
    else
        window->setChooseTribeGUI();


    window->show();

    //qInfo("before");

    int res = app.exec();

    //qInfo("after");

    // help multiaccount ? let say no for the moment
    if(screenView != SCREEN_VIEW_SELECT_LOCATION && screenView != SCREEN_VIEW_SELECT_TRIBE) // could also make "steps" if close during create account process
    {
        QSettings settings(settingsFile, QSettings::IniFormat);
        settings.setValue("timestampGameClosed", QVariant(QDateTime::currentSecsSinceEpoch()));
        //settings.setValue("screenView", QVariant(screenView));
        settings.setValue("tribe", QVariant(tribe));
        settings.setValue("nickname", QVariant(nickname));
        settings.setValue("timestampVillageStart", QVariant(timestampVillageStart));
        settings.sync();
    }

    return res;
}

QString getTribe(tribeEnum tribe)
{
    switch(tribe)
    {
        case TRIBE_GAULS:
            return "gauls";
        case TRIBE_ROMANS:
            return "romans";
        //case TRIBE_TEUTONS:
        default:
            return "teutons";
    }
}

QString getTribe()
{
    return getTribe(tribe);
}
