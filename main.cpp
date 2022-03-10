#include <QApplication>
#include <QtWidgets>
#include <QSettings>
#include <QFile>

//#include <QtNetwork>
//#include <functional>
#include "CryptoPlus.h"
#include "QRandomPlus.h"
#include "QFilePlus.h"

#include "resources.h"
#include "buildings.h"
#include "QPlus.h"
#include "main.h"
#include "server.h"
#include "client.h"
//#include "network.h"

QString USER_FOLDER = "user/";

screenViewEnum screenView,
               lastScreenView;
tribeEnum tribe;
QTranslator translator;
MyWindow* myWindow;
QString nickname;
QPair<quint16, quint16> coordinates;
quint8 screenViewFarmsIndex,
       screenViewBuildingsIndex;
Server* server;
Client* client;

void htmlGet(const QUrl &url, const std::function<void(const QString&)> &fun);

// instead of linking README.md etc to be able to commit and push without forcing if added content directly through web interface, could just use a .gitignore no ? if so upload tools
// gitignore doesn't seem to be the solution

// QtUPnP ? GUPnP ?
// in defaultNodes.txt could use domain name in order to be more stable - done
// just statistics and resources work could be a good beginning

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
  
    #if __APPLE__
        QDir::setCurrent("../../../");
    #endif

    QString languageFile = "travianblockchained_fr";
    if(translator.load(languageFile))
        app.installTranslator(&translator);
    else
        qInfo("languageFile couldn't be loaded !"); // how to inject languageFile value ?

    initializeRandom();

    //or just use downloaded file ? - downloaded file is more "trustable"
    //htmlGet({"https://raw.githubusercontent.com/Benjamin-Loison/Travian-blockchained/master/defaultNodes.txt"}, [](const QString &body){ qDebug() << body; });
    //QStringList defaultNodes = getFileContent("defaultNodes.txt");

    // could make a mode where when we build something we can't do anything else (for the moment) and so we are teleported in the future when it will be done and likewise we instantly play our all beginning of game
    bool runServer = true,
         runClient = false;
    if(runServer)
    {
        server = new Server;
    }
    if(runClient)
    {
        client = new Client;
        client->sendToServer("hello server !");
    }

    myWindow = new MyWindow();
    QString settingsFile = USER_FOLDER + "settings.ini";
    if(QFile::exists(settingsFile))
    {
        QSettings settings(settingsFile, QSettings::IniFormat); // could store private and public key within settings ? I don't know whether or not OpenSSL is designed to accept private and public keys as arguments and not files
        //screenView = static_cast<enum screenViewEnum>(settings.value("screenView").toInt()); // opening exactly where the user was, isn't that much interesting most of time
        tribe = static_cast<enum tribeEnum>(settings.value("tribe").toUInt());
        nickname = settings.value("nickname").toString();
        resourcesAmount[FREE_CROP] = settings.value("freeCrop").toUInt();
        population = settings.value("population").toUInt();
        culturePoints = settings.value("culturePoints").toUInt();
        timestampVillageStart = settings.value("timestampVillageStart").toUInt();
        timestampGameClosed = settings.value("timestampGameClosed").toUInt();
        QStringList initialResourcesAmountList = settings.value("initialResourcesAmount").toStringList(),
                    resourcesProductionList = settings.value("resourcesProduction").toStringList(),
                    farmsList = settings.value("farms").toStringList(),
                    buildingsList = settings.value("buildings").toStringList();
        for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
        {
            QString resourcesIndexStr = QString::number(resourcesIndex);
            initialResourcesAmount[resourcesIndex] = initialResourcesAmountList[resourcesIndex].toUInt();
            resourcesProduction[resourcesIndex] = resourcesProductionList[resourcesIndex].toUInt(); // or compute it manually - warning we take into account hero production even if none currently
        }
        for(quint8 farmsIndex = 0; farmsIndex < FARMS_NUMBER; farmsIndex++)
        {
            farms[farmsIndex] = farmsList[farmsIndex].toUInt();
        }
        for(quint8 buildingsIndex = 0; buildingsIndex < BUILDINGS_SLOTS_NUMBER; buildingsIndex++)
        {
            QString building = buildingsList[buildingsIndex];
            QStringList buildingParts = building.split(":");
            buildings[buildingsIndex] = qMakePair(buildingParts[0].toUInt(), buildingParts[1].toUInt());
        }
        bool updatedResourcesAmount = false;
        QString actionsFile = USER_FOLDER + "actions.txt",
                lastActionsLine = getLastLine(actionsFile);
        if(lastActionsLine != "")
        {
            //qInfo("yes0");
            QStringList parts = lastActionsLine.split(SEPARATOR);
            quint8 partsLength = parts.length();
            if(partsLength >= 4)
            {
                //qInfo("yes1");
                lastActionTime = parts[0].toUInt();
                lastAction = parts[1];
                //qInfo("last action loaded !");
                //qInfo(("!" + lastAction + "!").toStdString().c_str());

                // how to cleanly share this code with resources.cpp ?

                QStringList actionParts = lastAction.split(':');
                quint8 actionPartsLength = actionParts.length();
                if(actionPartsLength >= 2) // could remove useless checks (we assume the user doesn't modify incorrectly game files)
                {
                    //qInfo("yes2");
                    quint8 farmsIndex = actionParts[1].toUInt(),
                           farmLevel = farms[farmsIndex] + 1,
                           farmType = farmsTypes[farmsIndex];

                    quint32 timeRequired = getTimeForBuilding(farmType, farmLevel),
                            currentTime = getSeconds(),
                            timeFinished = lastActionTime + timeRequired;
                    if(timestampGameClosed < timeFinished)
                    {
                        //qInfo("yes3");
                        // instead of loosing a resource here and there could work with double and just show floor integer everywhere
                        // de là à intégrer des delta infinitésimaux à chaque frame ça serait encore pire
                        // et temps que l'on floor il n'y a pas de problème de fausse triche possible ce qui est le principal
                        //lastActionTime = 0;
                        if(timeFinished < currentTime)
                        {
                            //qInfo("yes4");
                            //qInfo(("0: " + QString::number(resourcesAmount[CLAY])).toStdString().c_str());
                            updateResourcesAmount(timeFinished);
                            //qInfo(("1: " + QString::number(resourcesAmount[CLAY])).toStdString().c_str());
                            timestampGameClosed = timeFinished;
                            initialResourcesAmount[farmType] = resourcesAmount[farmType];
                            buildingGetBuilt(farmsIndex);
                            //qInfo(QString::number(farmsIndex).toStdString().c_str());
                            updateResourcesAmount();
                            //qInfo(("2: " + QString::number(resourcesAmount[CLAY])).toStdString().c_str());
                            updatedResourcesAmount = true;
                            lastActionTime = 0;
                        }
                    }
                    else
                    {
                        lastActionTime = 0;
                    }
                }
            }
        }
        if(!updatedResourcesAmount)
        {
            updateResourcesAmount();
        }
        myWindow->startGame(true);
    }
    else
        myWindow->setChooseTribeGUI();

    myWindow->show();

    int res = app.exec();

    if(screenView != SCREEN_VIEW_SELECT_LOCATION && screenView != SCREEN_VIEW_SELECT_TRIBE) // could also make "steps" if close during create account process
    {
        QSettings settings(settingsFile, QSettings::IniFormat);
        settings.setValue("timestampGameClosed", QVariant(getSeconds()));
        QStringList initialResourcesAmountList,
                    resourcesProductionList,
                    farmsList,
                    buildingsList;
        for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
        {
            // could only save non null values and when loading could give default value if not found
            QString resourcesIndexStr = QString::number(resourcesIndex);
            initialResourcesAmountList.append(QString::number(resourcesAmount[resourcesIndex]));
            resourcesProductionList.append(QString::number(resourcesProduction[resourcesIndex]));
        }
        // QStringList stores as =x0, x1, x2, x3 unless one contains a ','
        // QVariant::fromValue<QList<quint32>>(initialResourcesAmountList) is far less clear for a human reader
        // QString stores as ="x0,x1,x2,x3", we have quotes because it contains a comma it seems
        // let's use QStringList because it's easier in the code than a QString
        settings.setValue("initialResourcesAmount", QVariant(initialResourcesAmountList)); // instead of storing each value in a variable, likewise the settings file isn't having a lot of lines even if with a lot of values in array it is maybe less clear for a reader - in fact with less lines it is also in general more clear to a human reader
        settings.setValue("resourcesProduction", QVariant(resourcesProductionList));

        for(quint8 farmsIndex = 0; farmsIndex < FARMS_NUMBER; farmsIndex++)
        {
            farmsList.append(QString::number(farms[farmsIndex]));
        }
        settings.setValue("farms", QVariant(farmsList));

        for(quint8 buildingsIndex = 0; buildingsIndex < BUILDINGS_SLOTS_NUMBER; buildingsIndex++)
        {
            QPair<quint8, quint8> building = buildings[buildingsIndex];
            buildingsList.append(QString::number(building.first) + ":" + QString::number(building.second));
        }
        settings.setValue("buildings", QVariant(buildingsList));
        //settings.setValue("screenView", QVariant(screenView));
        settings.setValue("tribe", QVariant(tribe));
        settings.setValue("nickname", QVariant(nickname));
        settings.setValue("freeCrop", QVariant(resourcesAmount[FREE_CROP]));
        settings.setValue("population", QVariant(population));
        settings.setValue("culturePoints", QVariant(culturePoints));
        settings.setValue("timestampVillageStart", QVariant(timestampVillageStart));
        settings.sync();
    }

    return res;
}

// https://stackoverflow.com/a/24966317/7123660
/*void htmlGet(const QUrl &url, const std::function<void(const QString&)> &fun)
{
   QScopedPointer<QNetworkAccessManager> manager(new QNetworkAccessManager);
   QNetworkReply *response = manager->get(QNetworkRequest(QUrl(url)));
   QObject::connect(response, &QNetworkReply::finished, [response, fun]{
      response->deleteLater();
      response->manager()->deleteLater();
      if (response->error() != QNetworkReply::NoError) return;
      auto const contentType =
            response->header(QNetworkRequest::ContentTypeHeader).toString();
      static QRegularExpression re("charset=([!-~]+)");
      auto const match = re.match(contentType);
      if (!match.hasMatch() || 0 != match.captured(1).compare("utf-8", Qt::CaseInsensitive)) {
         qWarning() << "Content charsets other than utf-8 are not implemented yet:" << contentType;
         return;
      }
      auto const html = QString::fromUtf8(response->readAll());
      fun(html); // do something with the data
   }) && manager.take();
}*/

QString getTribe(tribeEnum tribe)
{
    switch(tribe)
    {
        case TRIBE_GAULS:
            return QT_TRANSLATE_NOOP("tribes", "gauls"); // QT_TR_NOOP("gauls")
        case TRIBE_ROMANS:
            return QT_TRANSLATE_NOOP("tribes", "romans");
        default: // case TRIBE_TEUTONS:
            return QT_TRANSLATE_NOOP("tribes", "teutons");
    }
}

QString getTribe()
{
    return getTribe(tribe);
}

QString getScreenView()
{
    switch(screenView)
    {
        case SCREEN_VIEW_BUILDINGS:
            return "buildings";
        case SCREEN_VIEW_RESOURCES:
            return "resources";
        case SCREEN_VIEW_SELECT_LOCATION:
            return "select location";
        case SCREEN_VIEW_SELECT_TRIBE:
            return "select tribe";
        default: // case SCREEN_VIEW_STATISTICS:
            return "statistics";
    }
}

QString getCoordinatesStr(QPair<quint16, quint16> coordinates)
{
    quint16 x = coordinates.first,
            y = coordinates.second;
    QString xStr = QString::number(x),
            yStr = QString::number(y);
    return "(" + xStr + "|" + yStr + ")";
}

QString getCoordinatesStr()
{
    return getCoordinatesStr(coordinates);
}
