#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QByteArray>
#include <QJsonObject>

class Util
{
public:
    Util();
    static void disown(QString &cmd, QStringList &arguments);
    static bool getPID(QString &name);
    QByteArray readFile(QString &inputFile);
    QJsonObject readJSONFile(QString &inputFile);
    QString readRawFile(QString &inputFile);
    void saveFile(QString &data, QString &exportFile);
    void saveJSONFile(QJsonObject &data, QString &exportFile);
    void createDir(QString &directory);
    void setupLogging();
    static QString joinPath(QString oPath, QStringList addition);
    static QString which(QString &program);
    static void run(QString command, QStringList args);
    static void pOpen(QString command, QStringList args);
    static QByteArray checkOutput(QString command, QStringList arguments);
};

#endif // UTIL_H
