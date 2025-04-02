#ifndef SETTINGS_H
#define SETTINGS_H

#include <QProcessEnvironment>
#include <QSysInfo>
#include <filesystem>

class Setting {
public:
    static const QProcessEnvironment env;
    static const QString HOME;
    static const QString XDG_CACHE_DIR;
    static const QString XDG_CONFIG_DIR;
    static const QString CACHE_DIR;
    static const QString CONF_DIR;
    static const QString TERM;
    // os.path.dirname(__file__)
    static const std::filesystem::path cPath;
    static const QString MODULE_DIR;
    // platform.uname()[0]
    static const QString OS;
    static const QString version = "0.1.0";
    static const QString cacheVersion = "1.0.0";
};

#endif // SETTINGS_H
