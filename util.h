#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QByteArray>
#include <QJsonObject>

class Util
{
public:
    Util();
    void disown(QString &cmd);
    bool getPID(QString &name);
    QByteArray readFile(QString &inputFile);
    QJsonObject readJSONFile(QString &inputFile);
    QString readRawFile(QString &inputFile);
    void saveFile(QString &data, QString &exportFile);
    void saveJSONFile(QJsonObject &data, QString &exportFile);
    void createDir(QString &directory);
    void setupLogging();
};

#endif // UTIL_H
