#include <QJsonArray>
#include <QDir>
#include "../include/appexception.h"
#include "../include/color.h"
#include "../include/logging.h"
#include "../include/theme.h"

Theme::Theme() = default;

/**
 * Save colors to a theme file
 * @param colors
 * @param name
 * @param light
 */
void Theme::save(QJsonObject &colors, QString &name, bool light) {
    QString themeFile = name.append(".json");
    QString mode = light ? "light" : "dark";
    QString themePath = Util::joinPath(Setting::CONF_DIR, QStringList() << "colorschemes" << mode << themeFile);

    Util::saveJSONFile(colors, themePath);
}

/**
 * Import colorscheme from json file
 * @param inputFile
 * @param light
 * @return
 */
QJsonObject Theme::import(QString &inputFile, bool light) {
    QString lightDir = Util::joinPath(Setting::CONF_DIR, QStringList() << "colorschemes" << "light");
    QString darkDir = Util::joinPath(Setting::CONF_DIR, QStringList() << "colorschemes" << "dark");
    QString darkDirModules = Util::joinPath(Setting::MODULE_DIR, QStringList() << "colorschemes" << "dark");
    QString lightDirModules = Util::joinPath(Setting::MODULE_DIR, QStringList() << "colorschemes" << "light");
    Util::createDir(lightDir);
    Util::createDir(darkDir);
    // Create module theme directories
    Util::createDir(darkDirModules);
    Util::createDir(lightDirModules);

    QString themeFile;

    // Find theme file
    if (inputFile == "random" | inputFile == "random_dark") {
        themeFile = getRandomTheme();
    } else if (inputFile == "random_light") {
        themeFile = getRandomTheme(false);
    } else if (inputFile == "random_user") {
        themeFile = getRandomUserTheme();
    } else {
        QString mode = light ? "light" : "dark";
        QString themeName = inputFile.endsWith(".json") ? inputFile : inputFile.append(".json");
        QString userThemeFile = Util::joinPath(Setting::CONF_DIR, QStringList() << "colorschemes" << mode << themeName);
        themeFile = Util::joinPath(Setting::MODULE_DIR, QStringList() << "colorschemes" << mode << themeName);

        QFileInfo ut_info(userThemeFile);
        QFileInfo i_info(inputFile);
        if (ut_info.exists() & ut_info.isFile()) {
            themeFile = ut_info.absoluteFilePath();
        }

        if (i_info.exists() & i_info.isFile()) {
            themeFile = i_info.absoluteFilePath();
        }
    }

    // Parse the theme file
    QFileInfo t_info(themeFile);
    if (t_info.exists() & t_info.isFile()) {
        QString logMsg = QString("Set theme to \033[1;37m%1\033[0m.").arg(t_info.fileName());
        Logging::info(logMsg);
        QString fileName = t_info.absoluteFilePath();
        QString path = Util::joinPath(Setting::CACHE_DIR, QStringList() << "last_used_theme");
        Util::saveFile(fileName, path, false);

        QString themePath = t_info.absoluteFilePath();
        return parse(themePath);
    }


    return {};
}

/**
 * Get a random theme file.
 * @brief Theme::getRandomTheme
 * @param mode
 * @return
 */
QString Theme::getRandomTheme(bool mode) {
    QList<QString> themes = listThemes(mode);
    int randomIdx = Util::getRandomInt(0, (int)themes.size());

    return themes.at(randomIdx);
}

/**
 * Get a random theme file from user theme directories.
 * @brief Theme::getRandomUserTheme
 * @return
 */
QString Theme::getRandomUserTheme() {
    QList<QString> themes = listUserThemes();
    int randomIdx = Util::getRandomInt(0, (int)themes.size());

    return themes.at(randomIdx);
}

QJsonObject Theme::parse(QString &themeFile) {
    QJsonObject jsonData = Util::readJSONFile(themeFile);
    if (!jsonData.contains("wallpaper")) {
        jsonData.insert("wallpaper", QJsonValue("None"));
    }

    if (!jsonData.contains("alpha")) {
        Color color;
        jsonData.insert("alpha", color.alphaValue);
    }

    // Terminal.sexy format
    if (jsonData.contains("color")) {
        return terminalSexyToWal(jsonData);
    }

    return jsonData;
}

/**
 * List user theme files
 * @brief Theme::listUserThemes
 * @return
 */
QList<QString> Theme::listUserThemes() {
    QString themeDir  = Util::joinPath(Setting::CONF_DIR, QStringList() << "colorschemes");

    QList<QString> themes = listUtil(themeDir, true) + listUtil(themeDir, false);

    return themes;
}

/**
 * List all installed theme files
 * @brief Theme::listThemes
 * @return
 */
QList<QString> Theme::listThemes(bool mode) {
    QString themeDir  = Util::joinPath(Setting::MODULE_DIR, QStringList() << "colorschemes");

    return listUtil(themeDir, mode);
}

/**
 * Convert terminal.sexy json schema to wal
 * @brief Theme::terminalSexyToWal
 * @param data
 * @return
 */
QJsonObject Theme::terminalSexyToWal(QJsonObject &data) {
    QJsonObject result = {};
    result.insert("colors", {});
    QJsonObject special {
        {"foreground", data["foreground"]},
        {"background", data["background"]},
        {"cursor", data.value("color").toObject().value("9")}
    };
    result.insert("special", special);

    QJsonArray colorArray = data.value("color").toArray();
    QJsonObject c {};
    for (int i = 0; i < colorArray.size(); i++) {
        QString key = QString("color%1").arg(i);
        c.insert(key, colorArray[i]);
    }
    result.insert("colors", c);

    return result;
}

/**
 * List all themes prettily
 * @brief Theme::listAllThemes
 */
void Theme::listAllThemes() {
    QList<QString> darkThemes = listThemes();
    QList<QString> lightThemes = listThemes(false);
    QList<QString> userThemes = listUserThemes();

    foreach (QString entry, darkThemes) {
        entry.replace(".json", "");
    }

    foreach (QString entry, lightThemes) {
        entry.replace(".json", "");
    }

    foreach (QString entry, userThemes) {
        entry.replace(".json", "");
    }

    QString lastUsedTheme;
    try {
        QString path = Util::joinPath(Setting::CACHE_DIR, QStringList() << "last_used_theme");
        lastUsedTheme = Util::readRawFile(path);
    } catch (AppException &exception) {
        lastUsedTheme = "";
    }
    QTextStream out(stdout);
    if (!userThemes.isEmpty()) {
        out << "\033[1;32mUser Themes\033[0m:" << Qt::endl;
        foreach (const QString &entry, userThemes) {
            QString outStr = entry;
            if (entry == lastUsedTheme) {
                outStr.append(" (last used)");
            }
            out << outStr.prepend("\n - ");
        }
    }

    if (!darkThemes.isEmpty()) {
        out << "\033[1;32mUser Themes\033[0m:" << Qt::endl;
        foreach (const QString &entry, darkThemes) {
            QString outStr = entry;
            if (entry == lastUsedTheme) {
                outStr.append(" (last used)");
            }
            out << outStr.prepend("\n - ");
        }
    }

    if (!lightThemes.isEmpty()) {
        out << "\033[1;32mUser Themes\033[0m:" << Qt::endl;
                foreach (const QString &entry, lightThemes) {
                QString outStr = entry;
                if (entry == lastUsedTheme) {
                    outStr.append(" (last used)");
                }
                out << outStr.prepend("\n - ");
            }
    }

    out << "\033[1;32mExtra\033[0m:" << Qt::endl;
    out << " - random (select a random dark theme)" << Qt::endl;
    out << " - random_dark (select a random dark theme)" << Qt::endl;
    out << " - random_light (select a random light theme)" << Qt::endl;
    out << " - random_user (select a random user theme)" << Qt::endl;

}

/**
 * Theme listing utility. Returns a sorted list of themes.
 * @param dirName
 * @param mode
 * @return
 */
QList<QString> Theme::listUtil(QString &dirName, bool mode) {
    QString themeMode = mode ? "dark" : "light";
    QFileInfo themeDir(Util::joinPath(dirName, QStringList() << themeMode));

    if (!themeDir.exists() & !themeDir.isDir()) {
        std::string message = QString("%1 isn't a valid directory!").arg(themeDir.absolutePath()).toStdString();
        throw AppException(message);
    }


    QList<QString> themes;
    foreach (const auto &entry, themeDir.dir().entryInfoList()) {
        if (entry.isFile()) {
            themes.append(entry.absoluteFilePath());
        }
    }
    // Sort themes
    themes.sort();

    return themes;
}
