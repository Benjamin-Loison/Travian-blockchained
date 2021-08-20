#include <QApplication>
#include <QtWidgets>
#include <QSettings>
#include <QFile>

//#include <QtNetwork>
//#include <functional>

//#include <qrsaencryption.h>

#include "resources.h"
#include "QPlus.h"
#include "main.h"
//
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

// max 2048 bits https://github.com/QuasarApp/Qt-Secret/issues/91 - doesn't seem to be the case
// min 3072 recommended by NSA https://apps.nsa.gov/iaarchive/programs/iad-initiatives/cnsa-suite.cfm
// hash 384 recommended as minimum ?! https://www.keylength.com/fr/compare/

// RSA in seconds
// key size   2048  3072   4096   6144   8192
// genPairKey 2.119  6.974 44.000 31.384 243.044 (random process)
// encode     0.062  0.170  0.437  1.489   3.470
// decode     0.096  0.286  0.745  2.449   5.651
// sign       1.668  5.503 13.226 44.422 104.119
// checkSign  2.906 10.014 23.420 75.711 262.077

/*bool testEncryptAndDecryptExample() {

    QByteArray pub, priv;
    QRSAEncryption e(QRSAEncryption::Rsa::RSA_8192);
    qDebug() << getTime() << " 0"; // how to get millis ?
    e.generatePairKey(pub, priv); // or other rsa size
    qDebug() << getTime() << " 1"; // 2 s for 2048, 7 for 3072, 25->29->41 for 4096, 240->492->565 for 8192

    QByteArray msg = "test message";

    auto encryptMessage = e.encode(msg, pub);
    qDebug() << getTime() << " 2"; // 0 s for 2048, 1 for 3072, 0 for 4096, 3 for 8192
    //qInfo(encryptMessage.toHex()); // pas les mêmes je n'ai pas dû bien comprendre la signature RSA
    //qInfo(e.encode(msg, priv).toHex());

    if (encryptMessage == msg)
        return false;

    auto decodeMessage = e.decode(encryptMessage, priv);
    qDebug() << getTime() << " 3"; // 0 s for 2048, 1 for 4096, 5 for 8192

    return decodeMessage == msg;
}

bool testExample() {
    QByteArray pub, priv;
    //qDebug() << getTime() << " a";
    QRSAEncryption e(QRSAEncryption::Rsa::RSA_8192);
    qDebug() << getTime() << " b";
    e.generatePairKey(pub, priv); // or other rsa size
    qDebug() << getTime() << " bc";

    QByteArray msg = "test message";

    auto signedMessage = e.signMessage(msg, priv); // 1 s for 2048, 12 for 4096, 97 for 8192
    qDebug() << getTime() << " c";

    if (e.checkSignMessage(signedMessage, pub)) { // 3 s for 2048, 22 for 4096, 170 for 8192
        qInfo() <<" message signed success";
        qDebug() << getTime() << " d";
        return true;
    }
    qDebug() << getTime() << " e";

    return false;
}*/

// with this https://crypto.stackexchange.com/q/9896 I could reimplement checkSignMessage ? how is it ?
// https://github.com/QuasarApp/Qt-Secret/blob/main/src/Qt-RSA/qrsaencryption.cpp

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

    /*if (testEncryptAndDecryptExample()) {
           qInfo() << "Success!";
       }*/
    /*if (testExample()) {
            qInfo() <<"success!";
        }*/



    window = new MyWindow();
    QString settingsFile = "user/settings.ini";
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
