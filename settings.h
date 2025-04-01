#ifndef SETTINGS_H
#define SETTINGS_H

#include <QProcessEnvironment>
#include <filesystem>
#include "util.h"

const QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
Util util;

QString HOME = env.contains("HOME") ? env.value("HOME") : env.value("USERPROFILE");
QString XDG_CACHE_DIR = env.contains("XDG_CACHE_HOME") ? env.value("XDG_CACHE_HOME") : util.joinPath(HOME, QString(".cache"));
QString XDG_CONFIG_DIR = env.contains("XDG_CONFIG_HOME") ? env.value("XDG_CONFIG_HOME") : util.joinPath(HOME, QString(".config"));
QString CACHE_DIR = env.contains("PYWAL_CACHE_DIR") ? env.value("PYWAL_CACHE_DIR") : util.joinPath(XDG_CACHE_DIR, QString("walpp"));
QString CONF_DIR = util.joinPath(XDG_CONFIG_DIR, QString("walpp"));

// TODO: Implement os.path.dirname(__file__)
auto cPath = std::filesystem::current_path();
QString MODULE_DIR = QString::fromStdString(cPath.string());
// TODO: platform.uname()[0]
QString OS = "";

#endif // SETTINGS_H
