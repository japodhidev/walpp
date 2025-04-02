#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include "settings.h"

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
    static QList<QString> listBackends();
    static QString normalizeImgPath(QString &img);
    QJsonObject colorsToMap(QList<QString> colors, QString &img);
    QList<QString> genericAdjust(QList<QString> colors, bool light);
    QList<QString> saturateColors(QList<QString> colors, int amount);
    QList<QString> cacheFname(QString &img, QString &backend, bool light, QString &cacheDir, QString sat = "");
    QString getBackend(QString &backend);
    void palette();
    QJsonObject getColors(QString img, bool light = false, QString backend = "wal", QString cacheDir = Setting::CACHE_DIR,  QString sat = "");
};

#endif // UTIL_H
