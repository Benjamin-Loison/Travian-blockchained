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

    window = new MyWindow();
    QString settingsFile = "settings.ini";
    if(QFile::exists(settingsFile))
    {
        QSettings settings(settingsFile, QSettings::IniFormat);
        //screenView = static_cast<enum screenViewEnum>(settings.value("screenView").toInt()); // opening exactly where the user was, isn't that much interesting most of time
        tribe = static_cast<enum tribeEnum>(settings.value("tribe").toUInt());
        nickname = settings.value("nickname").toString();
        timestampVillageStart = settings.value("timestampVillageStart").toUInt();
        for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
        {
            initialResourcesAmount[resourcesIndex] = settings.value("initialResources" + QString::number(resourcesIndex) + "Amount").toUInt();
        }
        for(quint8 farmsIndex = 0; farmsIndex < FARMS_NUMBER; farmsIndex++)
        {
            farms[farmsIndex] = settings.value("farms" + QString::number(farmsIndex)).toUInt();
        }
        window->startGame(true);
    }
    else
        window->setChooseTribeGUI();


    window->show();

    int res = app.exec();

    if(screenView != SCREEN_VIEW_SELECT_LOCATION && screenView != SCREEN_VIEW_SELECT_TRIBE) // could also make "steps" if close during create account process
    {
        QSettings settings(settingsFile, QSettings::IniFormat);
        settings.setValue("timestampGameClosed", QVariant(QDateTime::currentSecsSinceEpoch()));
        for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
        {
            // could only save non null values and when loading could give default value if not found
            settings.setValue("initialResources" + QString::number(resourcesIndex) + "Amount", QVariant(resourcesAmount[resourcesIndex]));
        }
        for(quint8 farmsIndex = 0; farmsIndex < FARMS_NUMBER; farmsIndex++)
        {
            settings.setValue("farms" + QString::number(farmsIndex), QVariant(farms[farmsIndex]));
        }
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
        default: // case TRIBE_TEUTONS:
            return "teutons";
    }
}

QString getTribe()
{
    return getTribe(tribe);
}
