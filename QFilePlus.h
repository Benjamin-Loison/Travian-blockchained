#ifndef DEF_QFILEPLUS_H
#define DEF_QFILEPLUS_H

#include <QStringList>
#include <QIODevice>

QString getFileContentAsHex(QString filePath),
        getLastLine(QString filePath);
QStringList getFileContent(QString path);
bool saveAction(quint32 timestamp, QString action, QString nonce, QString hash),
     writeFile(QString filePath, QString toWrite, QIODeviceBase::OpenMode flags = QIODevice::WriteOnly),
     removeFile(QString filePath);

#endif
