#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QProcessEnvironment>
#include <QString>
#include "settings.h"

class Wallpaper
{
public:
    Wallpaper();
    QProcessEnvironment procesEnv;

    QString getDesktopEnv();
    void xfconf(QString &img);
    void setWMWallpaper(QString &img);
    void setDesktopWallpaper(QString &desktop, QString &img);
    void setMacWallpaper(QString &img);
    void setWinWallpaper(QString &img);
    void change(QString &img);
    QString get(std::string cacheDir = CACHE_DIR.toStdString());
};

#endif // WALLPAPER_H
