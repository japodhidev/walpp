#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <set>
#include "haishoku.h"
#include "settings.h"


class Util
{
public:
    Util();
    static void disown(QString &cmd, QStringList &arguments);
    static bool getPID(QString &name);
    static QByteArray readFile(QString &inputFile);
    static QJsonObject readJSONFile(QString &inputFile);
    static QString readRawFile(QString &inputFile);
    static QList<QString> readRawFileToList(QString &inputFile);
    static void saveFile(QString &data, QString &exportFile, bool mkDir = true);
    static void saveFileLines(QList<QString> &data, QString &exportFile, bool mkDir = true);
    static void saveJSONFile(QJsonObject &data, QString &exportFile);
    static void createDir(QString &directory);
    static void removeDirFiles(const QString& directory);
    static QString joinPath(QString oPath, QStringList addition);
    static QString which(const QString& program);
    static void run(const QString& command, QStringList &args);
    static void pOpen(const QString& command, const QStringList& args);
    static QByteArray checkOutput(const QString& command, const QStringList& arguments);
    static QList<QString> listBackends();
    static QString normalizeImgPath(QString &img);
    static QJsonObject colorsToMap(const QList<QString>& colors, QString &img);
    static QList<QString> genericAdjust(QList<QString> colors, bool light);
    static QList<QString> saturateColors(QList<QString> colors, int amount);
    static QList<QString> cacheFileName(QString &img, QString &backend, bool light, QString &cacheDir, QString sat = "");
    static QString getBackend(QString &backend);
    static void palette();
    static QJsonObject getColors(QString img, bool light = false, QString backend = "wal", QString cacheDir = Setting::CACHE_DIR,  QString sat = "");
    static int getRandomInt(int min, int max);
    static QList<QString> strVectorToQList(std::vector<std::string> &items);
    static std::vector<std::string> strQListToVector(QList<QString> &items);
};

#endif // UTIL_H
