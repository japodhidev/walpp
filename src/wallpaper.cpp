#include "../include/wallpaper.h"
#include "../include/util.h"
#include "../include/appexception.h"
#include <QUrl>
#include <QFileInfo>

Wallpaper::Wallpaper() {
    this->processEnv = QProcessEnvironment::systemEnvironment();
}

/**
 * Identify the currently running desktop environment.
 * @brief Wallpaper::getDesktopEnv
 */
QString Wallpaper::getDesktopEnv() const {
    if (this->processEnv.contains("XDG_CURRENT_DESKTOP")) {
        return this->processEnv.value("XDG_CURRENT_DESKTOP");
    }

    if (this->processEnv.contains("DESKTOP_SESSION")) {
        return this->processEnv.value("DESKTOP_SESSION");
    }

    if (this->processEnv.contains("GNOME_DESKTOP_SESSION_ID")) {
        return "GNOME";
    }

    if (this->processEnv.contains("MATE_DESKTOP_SESSION_ID")) {
        return "MATE";
    }

    if (this->processEnv.contains("SWAYSOCK")) {
        return "SWAY";
    }

    if (this->processEnv.contains("DESKTOP_STARTUP_ID") & this->processEnv.value("DESKTOP_SESSION").contains("awesome")) {
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
    QString command = "";

    if (!Util::which("feh").isEmpty()) {
        // feh
        command = "feh";
        Util::disown(command, QStringList() << "--bg-fill" << img);
    } else if (!Util::which("xwallpaper").isEmpty()) {
        // xwallpaper
        command = "xwallpaper";
        Util::disown(command, QStringList() << "--zoom" << img);
    } else if (!Util::which("hsetroot").isEmpty()) {
        // hsetroot
        command = "hsetroot";
        Util::disown(command, QStringList() << "-fill" << img);
    } else if (!Util::which("nitrogen").isEmpty()) {
        // nitrogen
        command = "nitrogen";
        Util::disown(command, QStringList() << "--set-zoom-fill" << img);
    } else if (!Util::which("bgs").isEmpty()) {
        // bgs
        command = "bgs";
        Util::disown(command, QStringList() << "-z" << img);
    } else if (!Util::which("habak").isEmpty()) {
        // habak
        command = "habak";
        Util::disown(command, QStringList() << "-mS" << img);
    } else if (!Util::which("display").isEmpty()) {
        // display
        command = "display";
        Util::disown(command, QStringList() << "-backdrop" << "-window" << "root" << img);
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
    QString command;
    // urllib.parse.quote(img)
    QUrl imgUrl = QUrl(img);
    QString file = QString("file://").append(imgUrl.toEncoded());

    if (desktop.contains("xfce") | desktop.contains("xubuntu")) {
        this->xfconf(img);
    } else if(desktop.contains("muffin") | desktop.contains("cinnamon")) {
        command = "gsettings";
        Util::disown(command, QStringList() << "set" << "org.cinnamon.desktop.background" << "picture-uri" <<  file);
    } else if(desktop.contains("gnome") | desktop.contains("unity")) {
        command = "gsettings";
        Util::disown(command, QStringList() << "set" << "org.gnome.desktop.background" << "picture-uri" <<  file);
    } else if (desktop.contains("mate")) {
        command = "gsettings";
        Util::disown(command, QStringList() << "set" << "org.mate.background" << "picture-filename" << img);
    } else if (desktop.contains("sway")) {
        command = "swaymsg";
        Util::disown(command, QStringList() << "output" << "*" << "bg" << img << "fill");
    } else if(desktop.contains("awesome")) {
        command = "awesome-client";
        // TODO: "require('gears').wallpaper.maximized('{img}')".format(**locals())
        QString cmd = QString("require('gears').wallpaper.maximized('%1')").arg(img);
        Util::disown(command, QStringList() << cmd);
    } else if (desktop.contains("kde")) {
        command = "qdbus";
        QString str0 = QString(R"("General");d.writeConfig("Image", "%1")};)").arg(img);
        QString str = QString("%1%2%3%4")
            .arg(
                "var allDesktops = desktops();for (i=0;i<allDesktops.length;i++){",
                "d = allDesktops[i];d.wallpaperPlugin = \"org.kde.image\";",
                R"(d.currentConfigGroup = Array("Wallpaper", "org.kde.image")",
                str0);
            // .arg("d = allDesktops[i];d.wallpaperPlugin = \"org.kde.image\";")
            // .arg("d.currentConfigGroup = Array(\"Wallpaper\", \"org.kde.image\"")
            // .arg(str0);
        Util::disown(command, QStringList() << "org.kde.plasmashell" << "/PlasmaShell" << "org.kde.PlasmaShell.evaluateScript" << str);
    } else {
        this->setWMWallpaper(img);
    }
}

/**
 * FIXME: Implement setting wallpaper in Windows
 * @brief Wallpaper::setWinWallpaper
 * @param img
 */
void Wallpaper::setWinWallpaper(QString &img) {}

/**
 * FIXME: FIXME: Implement setting wallpaper in MacOS
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
        std::string message = "Unexpected wallpaper argument specified. Expected a file.";
        throw AppException(message);
    }

    QString desktop = this->getDesktopEnv();

    if (Setting::OS.toLower() == "darwin") {
        setMacWallpaper(img);
    } else if (Setting::OS.toLower() == "windows") {
        setWinWallpaper(img);
    } else {
        setDesktopWallpaper(desktop, img);
    }
}

/**
 * Get the current wallpaper
 * @brief Wallpaper::get
 */
QString Wallpaper::get(const std::string& cacheDir) {
    QString cDir = QString::fromStdString(cacheDir);
    QString newPath = Util::joinPath(cDir, QStringList() << "wal");
    QFileInfo currentWall = QFileInfo(newPath);

    if (!currentWall.isFile()) {
        std::string message = "Unexpected argument specified. Expected a file.";
        throw AppException(message);
    }
    QString absPath = currentWall.absoluteFilePath();
    QByteArray wall = Util::readFile(absPath);

    return QString{wall};
}
