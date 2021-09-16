#include "QFilePlus.h"
#include "resources.h"
#include "main.h"
#include <QIODevice>
#include <QMessageBox>
#include <QFile>

QString getFileContentAsHex(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)) return "error";
    QByteArray blob = file.readAll();
    return QString(blob.toHex());
}

QStringList getFileContent(QString filePath)
{
    QFile file(filePath);
    QStringList lines;
    if(!file.exists()) return lines;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "Error opening file", file.errorString());
    }
    else
    {
        while(!file.atEnd())
        {
            lines.append(file.readLine());
        }
        file.close();
    }
    return lines;
}

QString getLastLine(QString filePath) // could in the future just go to the end of file and then go back to the beginning and stop when find the first new line
{
    QStringList lines = getFileContent(filePath);
    quint32 linesLength = lines.length();
    QString lastLine = "";
    if(linesLength > 0)
    {
        lastLine = lines.back();
    }
    return lastLine;
}

bool writeFile(QString filePath, QString toWrite, QIODeviceBase::OpenMode flags)
{
    QFile file(filePath);
    if(file.open(flags))
    {
        QTextStream stream(&file);
        stream << toWrite;
        return true;
    }
    return false;
}

bool saveAction(quint32 timestamp, QString action, QString nonce, QString signature)
{
    QString filePath = USER_FOLDER + "actions.txt";
    bool doesFileExists = QFile::exists(filePath);
    QString toWrite = (doesFileExists ? "\n" : "") + QString::number(timestamp) + SEPARATOR + action + SEPARATOR + nonce + SEPARATOR + signature;
    bool res = writeFile(filePath, toWrite, QIODevice::WriteOnly | QIODevice::Append);
    if(res)
    {
        lastActionTime = timestamp;
        lastAction = action;
    }
    return res;
}

bool removeFile(QString filePath)
{
    return QFile(filePath).remove();
}
