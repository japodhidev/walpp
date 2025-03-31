#include "wallpaper.h"
#include "util.h"
#include "appexception.h"
#include "settings.h"
#include <QUrl>
#include <QFile>
#include <QFileInfo>

Wallpaper::Wallpaper() {
    this->procesEnv = QProcessEnvironment::systemEnvironment();
}

/**
 * Identify the currently running desktop environment.
 * @brief Wallpaper::getDesktopEnv
 */
QString Wallpaper::getDesktopEnv() {
    if (this->procesEnv.contains("XDG_CURRENT_DESKTOP")) {
        return this->procesEnv.value("XDG_CURRENT_DESKTOP");
    }

    if (this->procesEnv.contains("DESKTOP_SESSION")) {
        return this->procesEnv.value("DESKTOP_SESSION");
    }

    if (this->procesEnv.contains("GNOME_DESKTOP_SESSION_ID")) {
        return "GNOME";
    }

    if (this->procesEnv.contains("MATE_DESKTOP_SESSION_ID")) {
        return "MATE";
    }

    if (this->procesEnv.contains("SWAYSOCK")) {
        return "SWAY";
    }

    if (this->procesEnv.contains("DESKTOP_STARTUP_ID") & this->procesEnv.value("DESKTOP_SESSION").contains("awesome")) {
        return "AWESOME";
    }

    return "";
}

/**
 * Call xfconf to set the wallpaper on XFCE.
 * @brief Wallpaper::xfconf
 * @param img
 */
void Wallpaper::xfconf(QString &img) {}

/**
 * Set the wallpaper for non desktop environments.
 * @brief Wallpaper::setWMWallpaper
 * @param img
 */
void Wallpaper::setWMWallpaper(QString &img) {
    // TODO: Find/develop a suitable replacement for shutil.which()
    bool shutil = false;
    QString command = "";
    Util util;

    if (shutil) {
        // feh
        command = "feh";
        util.disown(command, QStringList() << "--bg-fill" << img);
    } else if (shutil) {
        // xwallpaper
        command = "xwallpaper";
        util.disown(command, QStringList() << "--zoom" << img);
    } else if (shutil) {
        // hsetroot
        command = "hsetroot";
        util.disown(command, QStringList() << "-fill" << img);
    } else if (shutil) {
        // nitrogen
        command = "nitrogen";
        util.disown(command, QStringList() << "--set-zoom-fill" << img);
    } else if (shutil) {
        // bgs
        command = "bgs";
        util.disown(command, QStringList() << "-z" << img);
    } else if (shutil) {
        // habak
        command = "habak";
        util.disown(command, QStringList() << "-mS" << img);
    } else if (shutil) {
        // display
        command = "display";
        util.disown(command, QStringList() << "-backdrop" << "-window" << "root" << img);
    } else {
        std::string message = "No wallpaper setter found.";
        throw AppException(message);
    }
}

/**
 * Set the wallpaper on macOS
 * @brief Wallpaper::setDesktopWallpaper
 * @param desktop
 * @param img
 */
void Wallpaper::setDesktopWallpaper(QString &desktop, QString &img) {
    desktop = desktop.toLower();
    Util util;
    QString command;
    // urllib.parse.quote(img)
    QUrl imgUrl = QUrl(img);
    QString file = QString("file://").append(imgUrl.toEncoded());

    if (desktop.contains("xfce") | desktop.contains("xubuntu")) {
        this->xfconf(img);
    } else if(desktop.contains("muffin") | desktop.contains("cinnamon")) {
        command = "gsettings";
        util.disown(command, QStringList() << "set" << "org.cinnamon.desktop.background" << "picture-uri" <<  file);
    } else if(desktop.contains("gnome") | desktop.contains("unity")) {
        command = "gsettings";
        util.disown(command, QStringList() << "set" << "org.gnome.desktop.background" << "picture-uri" <<  file);
    } else if (desktop.contains("mate")) {
        command = "gsettings";
        util.disown(command, QStringList() << "set" << "org.mate.background" << "picture-filename" << img);
    } else if (desktop.contains("sway")) {
        command = "swaymsg";
        util.disown(command, QStringList() << "output" << "*" << "bg" << img << "fill");
    } else if(desktop.contains("awesome")) {
        command = "awesome-client";
        // TODO: "require('gears').wallpaper.maximized('{img}')".format(**locals())
        QString cmd = QString("require('gears').wallpaper.maximized('%1')").arg(img);
        util.disown(command, QStringList() << cmd);
    } else if (desktop.contains("kde")) {
        command = "qdbus";
        QString str0 = QString("\"General\");d.writeConfig(\"Image\", \"%1\")};").arg(img);
        QString str = QString("%1%2%3%4")
            .arg("var allDesktops = desktops();for (i=0;i<allDesktops.length;i++){")
            .arg("d = allDesktops[i];d.wallpaperPlugin = \"org.kde.image\";")
            .arg("d.currentConfigGroup = Array(\"Wallpaper\", \"org.kde.image\"")
            .arg(str0);
        util.disown(command, QStringList() << "org.kde.plasmashell" << "/PlasmaShell" << "org.kde.PlasmaShell.evaluateScript" << str);
    } else {
        this->setWMWallpaper(img);
    }
}

void Wallpaper::setWinWallpaper(QString &img) {}

/**
 * Set the wallpaper on Windows
 * @brief Wallpaper::setMacWallpaper
 * @param img
 */
void Wallpaper::setMacWallpaper(QString &img) {}

/**
 * Set the wallpaper
 * @brief Wallpaper::change
 * @param img
 */
void Wallpaper::change(QString &img) {
    QFileInfo fileInfo = QFileInfo(img);
    if (!fileInfo.isFile()) {
        std::string message = "Unexpected argument specified. Expected a file.";
        throw AppException(message);
    }

    QString desktop = this->getDesktopEnv();

    if (OS == "Darwin") {
        setMacWallpaper(img);
    } else if (OS == "Windows") {
        setWinWallpaper(img);
    } else {
        setDesktopWallpaper(desktop, img);
    }
}

/**
 * Get the current wallpaper
 * @brief Wallpaper::get
 */
QString Wallpaper::get(std::string cacheDir = CACHE_DIR.toStdString()) {
    QString cDir = QString::fromStdString(cacheDir);
    Util util;
    QString newPath = util.joinPath(cDir, QString("wal"));
    QFileInfo currentWall = QFileInfo(newPath);

    if (!currentWall.isFile()) {
        std::string message = "Unexpected argument specified. Expected a file.";
        throw AppException(message);
    }
    QString absPath = currentWall.absoluteFilePath();
    QByteArray wall = util.readFile(absPath);

    return QString(wall);
}
