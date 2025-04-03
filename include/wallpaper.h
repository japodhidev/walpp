#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QProcessEnvironment>
#include <QString>
#include "settings.h"

class Wallpaper
{
public:
    Wallpaper();
    QProcessEnvironment processEnv;

    QString getDesktopEnv();
    void xfconf(QString &img);
    void setWMWallpaper(QString &img);
    void setDesktopWallpaper(QString &desktop, QString &img);
    void setMacWallpaper(QString &img);
    void setWinWallpaper(QString &img);
    void change(QString &img);
    QString get(const std::string& cacheDir = Setting::CACHE_DIR.toStdString());
};

#endif // WALLPAPER_H
