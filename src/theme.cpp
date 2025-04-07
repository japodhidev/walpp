#include "../include/theme.h"
#include "../include/color.h"

Theme::Theme() = default;

/**
 * Save colors to a theme file
 * @param colors
 * @param name
 * @param light
 */
void Theme::save(QJsonObject colors, QString name, bool light) {
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
QJsonObject Theme::import(QString inputFile, bool light) {
    Util::createDir(Util::joinPath(Setting::CONF_DIR, QStringList() << "colorschemes" << "light"));
    Util::createDir(Util::joinPath(Setting::CONF_DIR, QStringList() << "colorschemes" << "dark"));
    // Create module directory
    Util::createDir(Util::joinPath(Setting::MODULE_DIR, QStringList() << "colorschemes" << "dark"));

    QString themeName = inputFile.append(".json");
    QString mode = light ? "light" : "dark";

    QString userThemeFile = Util::joinPath(Setting::CONF_DIR, QStringList() << "colorschemes" << mode << themeName);
    QString themeFile = Util::joinPath(Setting::MODULE_DIR, QStringList() << "colorschemes" << mode << themeName);

    // Find theme file
    if (inputFile == "random" | inputFile == "random_dark") {
        themeFile = getRandomTheme();
    } else if (inputFile == "random_light") {
        themeFile = getRandomTheme(false);
    } else if (inputFile == "random_user") {
        themeFile = getRandomUserTheme();
    }

    QFileInfo ut_info(userThemeFile);
    if (ut_info.exists() & ut_info.isFile()) {
        themeFile = ut_info.absoluteFilePath();
    }

    QFileInfo i_info(inputFile);
    if (i_info.exists() & i_info.isFile()) {
        themeFile = i_info.absoluteFilePath();
    }

    // Parse the theme file
    QTextStream out(stdout);
    QFileInfo t_info(themeFile);
    if (t_info.exists() & t_info.isFile()) {
        out << QString("Set theme to \033[1;37m%1\033[0m.").arg(t_info.baseName());
        QString baseName = t_info.baseName();
        QString path = Util::joinPath(Setting::CACHE_DIR, QStringList() << "last_used_theme");
        Util::saveFile(baseName, path);

        return parse(t_info.absoluteFilePath());
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
    return {};
}

/**
 * Get a random theme file from user theme directories.
 * @brief Theme::getRandomUserTheme
 * @return
 */
QString Theme::getRandomUserTheme() {
    return {};
}

QJsonObject Theme::parse(QString themeFile) {
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
    return {};
}

/**
 * List all installed theme files
 * @brief Theme::listThemes
 * @return
 */
QList<QString> Theme::listThemes() {
    return {};
}

/**
 * Convert terminal.sexy json schema to wal
 * @brief Theme::terminalSexyToWal
 * @param data
 * @return
 */
QJsonObject Theme::terminalSexyToWal(QJsonObject data) {
    QJsonObject result = {};
    result.insert("colors", QJsonValue({}));
    QJsonObject special {
        {"foreground", data["foreground"]},
        {"background", data["background"]},
        {"cursor", data.value("color").toObject().value("9")}
    };
    result.insert("special", special);

    auto colorArray = data.value("color").toArray();
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

}
