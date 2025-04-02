#include "../include/settings.h"
#include "../include/util.h"

Util util_o;
const QProcessEnvironment Setting::env =  QProcessEnvironment::systemEnvironment();
const QString Setting::HOME = Setting::env.contains("HOME") ? Setting::env.value("HOME") : Setting::env.value("USERPROFILE");
const QString Setting::XDG_CACHE_DIR = env.contains("XDG_CACHE_HOME")
                             ? env.value("XDG_CACHE_HOME")
                             : util_o.joinPath(HOME, QStringList() << ".cache");
const QString Setting::XDG_CONFIG_DIR = env.contains("XDG_CONFIG_HOME")
                              ? env.value("XDG_CONFIG_HOME")
                              : util_o.joinPath(HOME, QStringList() << ".config");
const QString Setting::CACHE_DIR =  env.contains("WALPP_CACHE_DIR")
                         ? env.value("WALPP_CACHE_DIR")
                         : util_o.joinPath(XDG_CACHE_DIR, QStringList() << "walpp");
const QString Setting::CONF_DIR = util_o.joinPath(XDG_CONFIG_DIR, QStringList() << "walpp");
const QString Setting::TERM = env.value("TERM");
const std::filesystem::path Setting::cPath = std::filesystem::current_path();
// const QString Setting::MODULE_DIR = QString::fromStdString(cPath.string());
const QString Setting::MODULE_DIR = util_o.joinPath(XDG_CONFIG_DIR, QStringList() << "walpp" << "modules");
const QString Setting::OS = QSysInfo::productType();
