#include <QApplication>
#include <QtWidgets>
#include <QSettings>
#include <QFile>

//#include <QtNetwork>
//#include <functional>

#include <qrsaencryption.h>

#include "resources.h"
#include "main.h"

screenViewEnum screenView;
tribeEnum tribe;
QTranslator translator;
MyWindow* window;
QString nickname;

void htmlGet(const QUrl &url, const std::function<void(const QString&)> &fun);

// instead of linking README.md etc to be able to commit and push without forcing if added content directly through web interface, could just use a .gitignore no ? if so upload tools
// gitignore doesn't seem to be the solution

// QtUPnP ? GUPnP ?
// let's choose 23900 as default port like my birthdate
// in defaultNodes.txt could use domain name in order to be more stable - done
// just statistics and resources work could be a good beginning

// max 2048 bits https://github.com/QuasarApp/Qt-Secret/issues/91
bool testEncryptAndDecryptExample() {

    QByteArray pub, priv;
    QRSAEncryption e(QRSAEncryption::Rsa::RSA_2048);
    e.generatePairKey(pub, priv); // or other rsa size

    QByteArray msg = "test message";

    auto encryptMessage = e.encode(msg, pub);

    if (encryptMessage == msg)
        return false;

    auto decodeMessage = e.decode(encryptMessage, priv);

    return decodeMessage == msg;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString languageFile = "travianblockchained_fr";
    if(translator.load(languageFile))
        app.installTranslator(&translator);
    else
        qInfo("languageFile couldn't be loaded !"); // how to inject languageFile value ?

    //or just use downloaded file ?
    //htmlGet({"https://raw.githubusercontent.com/Benjamin-Loison/Travian-blockchained/master/defaultNodes.txt"}, [](const QString &body){ qDebug() << body; });

    if (testEncryptAndDecryptExample()) {
            qInfo() << "Success!";
        }

    window = new MyWindow();
    QString settingsFile = "settings.ini";
    if(QFile::exists(settingsFile))
    {
        QSettings settings(settingsFile, QSettings::IniFormat);
        //screenView = static_cast<enum screenViewEnum>(settings.value("screenView").toInt()); // opening exactly where the user was, isn't that much interesting most of time
        tribe = static_cast<enum tribeEnum>(settings.value("tribe").toUInt());
        nickname = settings.value("nickname").toString();
        timestampVillageStart = settings.value("timestampVillageStart").toUInt();
        timestampGameClosed = settings.value("timestampGameClosed").toUInt();
        for(quint8 resourcesIndex = 0; resourcesIndex < RESOURCES_NUMBER; resourcesIndex++)
        {
            initialResourcesAmount[resourcesIndex] = settings.value("initialResources" + QString::number(resourcesIndex) + "Amount").toUInt();
        }
        for(quint8 farmsIndex = 0; farmsIndex < FARMS_NUMBER; farmsIndex++)
        {
            farms[farmsIndex] = settings.value("farms" + QString::number(farmsIndex)).toUInt();
        }
        updateResourcesAmount();
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
