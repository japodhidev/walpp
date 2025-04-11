#include "../include/logging.h"
#include "../include/reload.h"
#include "../include/util.h"
#include <QProcess>

Reload::Reload() = default;

/**
 * Load colors in tty
 * @brief Reload::tty
 * @param ttyReload
 */
void Reload::tty(bool ttyReload) {
    QString ttyScript = Util::joinPath(Setting::CACHE_DIR, QStringList() << "colors-tty.sh");
    QString term = QProcessEnvironment::systemEnvironment().value("TERM");

    if (ttyReload & term == "linux") {
        Util::pOpen("sh", QStringList() << ttyScript);
    }
}

/**
 * Merge the colors into the X database so new terminals use them
 * @brief xrdb
 * @param xrdbFiles
 */
void Reload::xrdb() {
    QStringList xrdbFiles = QStringList() << Util::joinPath(Setting::CACHE_DIR, QStringList() << "colors.Xresources");
    QString xrdb = "xrdb";
    if(!Util::which(xrdb).isEmpty() & Setting::OS.toLower() != "darwin") {
        foreach (const auto &entry, xrdbFiles) {
            Util::run(xrdb, QStringList() << "-merge" << "- quiet" << entry);
        }
    }
}

/**
 * Reload GTK theme on the fly
 * @brief Reload::gtk
 */
void Reload::gtk() {
    QString python2 = "python2";
    if (!Util::which(python2).isEmpty()) {
        QString gtkReload = Util::joinPath(Setting::MODULE_DIR, QStringList() << "scripts" << "gtk_reload.py");
        QString cmd = "python2";
        Util::disown(cmd, QStringList() << gtkReload);
    }
}

/**
 * Reload i3 colors
 * @brief Reload::i3
 */
void Reload::i3() {
    QString prog = "i3-msg";
    QString name = "i3";
    if (!Util::which(prog).isEmpty() & Util::getPID(name)) {
        Util::disown(prog, QStringList() << "reload");
    }
}

/**
 * Reload bspwm colors
 * @brief Reload::bspwm
 */
void Reload::bspwm() {
    QString prog = "bspc";
    QString name = "bspwm";
    if (!Util::which(prog).isEmpty() & Util::getPID(name)) {
        Util::disown(prog, QStringList() << "wm" << "-r");
    }

}

/**
 * Reload kitty colors
 * @brief Reload::kitty
 */
void Reload::kitty() {
    QString prog = "kitty";

    if(!Util::which(prog).isEmpty() & Util::getPID(prog) & Setting::TERM == "xterm-kitty") {
        Util::run(prog, QStringList() << "@" << "set-colors" << "--all" << Util::joinPath(Setting::CACHE_DIR, QStringList() << "colors-kitty.conf"));
    }
}

/**
 * Reload polybar colors
 * @brief Reload::polybar
 */
void Reload::polybar() {
    QString prog = "polybar";
    QString cmd = "pkill";
    if (!Util::which(prog).isEmpty() & Util::getPID(prog)) {
        Util::disown(cmd, QStringList() << "-USR1" << "polybar");
    }
}

/**
 * Reload sway colors
 * @brief Reload::sway
 */
void Reload::sway() {
    QString prog = "swaymsg";
    QString cmd = "sway";
    if (!Util::which(prog).isEmpty() & Util::getPID(cmd)) {
        Util::disown(prog, QStringList() << "reload");
    }
}

/**
 * Reload environment
 * @brief Reload::env
 */
void Reload::env(bool ttyReload) {
    xrdb();
    i3();
    bspwm();
    kitty();
    sway();
    polybar();
    Logging::info("Reloaded environment.");
    tty(ttyReload);
}