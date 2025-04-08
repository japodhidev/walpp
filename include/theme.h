//
// Created by sleek on 4/7/25.
//

#ifndef WALPP_THEME_H
#define WALPP_THEME_H

#include <cstdlib>
#include <QString>
#include <QJsonObject>
#include <QFileInfo>
#include <QTextStream>
#include "util.h"
#include "settings.h"

class Theme {
public:
    Theme();
    void save(QJsonObject colors, QString name, bool light = false);
    QJsonObject import(QString inputFile, bool light = false);
private:
    QString getRandomTheme(bool mode = true);
    QString getRandomUserTheme();
    QJsonObject parse(QString themeFile);
    QList<QString> listUserThemes();
    QList<QString> listThemes(bool mode = true);
    QJsonObject terminalSexyToWal(QJsonObject data);
    void listAllThemes();
    static QList<QString> listUtil(QString &dirName, bool mode);

};

#endif //WALPP_THEME_H
