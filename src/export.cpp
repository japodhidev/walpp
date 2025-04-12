#include "../include/export.h"
#include "../include/logging.h"

Export::Export() = default;

/**
 * Export all template files.
 * @param colors    [description]
 * @param outputDir [description]
 */
void Export::exportAll(QJsonObject &colors, const QString& outputDir) {
	QMap<QString, QString> mColors = flattenColors(colors);
	QString templateDir = Util::joinPath(Setting::MODULE_DIR, QStringList() << "templates");
	QString templateDirUser = Util::joinPath(Setting::CONF_DIR, QStringList() << "templates");
	Util::createDir(templateDirUser);

	QFileInfo tDir(templateDir);
    QFileInfo tDirUser(templateDirUser);

	QDirIterator dirIt(tDir.absoluteFilePath(), {}, QDir::Files, QDirIterator::Subdirectories);
	while (dirIt.hasNext()) {
	    QFileInfo f(dirIt.nextFileInfo());
	    QString fileName = f.fileName();
	    if (fileName != ".DS_Store" & !fileName.endsWith(".swp")) {
	    	QString path = Util::joinPath(outputDir, QStringList() << fileName);
            QString inputFile = f.absoluteFilePath();
            parseTemplate(mColors, inputFile, path);
	    }
    }

    QDirIterator dirItUser(tDirUser.absoluteFilePath(), {}, QDir::Files, QDirIterator::Subdirectories);
    while (dirItUser.hasNext()) {
        QFileInfo f(dirIt.nextFileInfo());
        QString fileName = f.fileName();
        if (fileName != ".DS_Store" & !fileName.endsWith(".swp")) {
            QString path = Util::joinPath(outputDir, QStringList() << fileName);
            QString inputFile = f.absoluteFilePath();
            parseTemplate(mColors, inputFile, path);
        }
    }

    Logging::info("Exported all files");
    Logging::info("Exported all user files");
}

/**
 * Export a single template file..
 * @param colors     [description]
 * @param exportType [description]
 * @param outputFile [description]
 */
void Export::exportSingle(QJsonObject &colors, QString &exportType, const QString& outputFile) {
	QMap<QString, QString> mColors = flattenColors(colors);
	QString templateName = getExportType(exportType);
	QString templateFile = Util::joinPath(Setting::MODULE_DIR, QStringList() << "templates" << templateName);
	QString oFile = !outputFile.isEmpty() ? outputFile : Util::joinPath(Setting::CACHE_DIR, QStringList() << templateName);

	QFileInfo tFile(templateFile);
	QTextStream out(stdout);
	if (tFile.isFile()) {
		parseTemplate(mColors, templateFile, oFile);
        Logging::info(QString("Exported %1").arg(exportType));
	} else {
		out << "Template '" << exportType << "' doesn't exist." << Qt::endl;
	}
}

/**
 * Prepare colors to be exported. Flatten dicts and convert colors to util.Color()
 */
QMap<QString, QString> Export::flattenColors(QJsonObject &colors) {
	QMap<QString, QString> allColors, result;
    QString alpha = colors.value("alpha").toString().isNull() ? QString("%1").arg(colors.value("alpha").toDouble()) : "99";
	allColors.insert("wallpaper", colors.value("wallpaper").toString());
	allColors.insert("alpha", alpha);
	
	QJsonObject spObj = colors["special"].toObject();
	QJsonObject colObj = colors["colors"].toObject();

	foreach (const QString &entry, spObj.keys()) {
		allColors.insert(entry, spObj.value(entry).toString());
	}

	foreach (const QString &entry, colObj.keys()) {
		allColors.insert(entry, colObj.value(entry).toString());
	}

	for (auto i = allColors.cbegin(), end = allColors.cend(); i != end; ++i) {
		if (i.key() == "wallpaper" | i.key() == "alpha") {
            result.insert(i.key(), i.value());
        } else {
            QString iStr = i.value();
            Color c(iStr);
            result.insert(i.key(), c.walColor.name(QColor::HexRgb));
        }
	}

	return result;
}

/**
 * Read template file, substitute markers and save the file elsewhere.
 * @param colors     [description]
 * @param inputFile  [description]
 * @param outputFile [description]
 */
void Export::parseTemplate(QMap<QString, QString> &colors, QString &inputFile, QString outputFile) {
	QList<QString> templateData = Util::readRawFileToList(inputFile);
    QList<QString> newData;
	QHash<QString, std::function<QString(Color)>> functions;
	functions["strip"] = [](const Color& value) {
        return value.strip();
    };
    functions["rgb"] = [](const Color& value) {
        return value.rgb();
    };
    // FIXME: Convert value back to int
   	functions["alpha_dec"] = [](const Color& value) {
        return QString("%1").arg(value.alphaDecimal());
    };
    functions["alpha"] = [](const Color& value) {
        return value.alpha();
    };
    functions["xrgba"] = [](Color value) {
        return value.xrgba();
    };

    QRegularExpression regex("(?<=(?<!\{))(\{([^{}]+)\})(?=(?!\}))");
	for (auto  entry : templateData) {
        QString newEntry = entry.replace("{{", "{").replace("}}", "}");
		// Search through all occurrences
		auto rit  = regex.globalMatch(entry);
		while(rit.hasNext()) {
            auto matched = rit.next();
			// Content to be processed e.g. {foreground.strip}
			QString subContent = matched.captured(1);
            QString bKey = matched.captured(2);
            // Split into color and function if a '.' is present
            QStringList parts = subContent.split('.');
            const QString &aKey = parts.at(0);
            // Change the value to be replaced to subContent at the presence of a '.'
            QString funcName = "";
            if (parts.size() > 1) {
                funcName = QString(parts.at(1)).remove("{").remove("}");
                bKey = QString(aKey).remove("{").remove("}");
            }

			// colors map contains the replacements
            // Check if the key exists in the replacements map
            if (colors.contains(bKey)) {
            	QString newValue = colors.value(bKey);
                // Exclude wallpaper & alpha values from conversion to colors
            	if (bKey == "wallpaper" | bKey == "alpha") {
                    newValue = colors.value(bKey);
                } else {
                    Color newColor(newValue);
                    // If a function is specified, apply it
                    if (!funcName.isEmpty() && functions.contains(funcName)) {
                        std::function<QString(Color)> func = functions[funcName];
                        newValue = func(newColor);
                    }
                }
                // Replace the matched content with the new value.
                newEntry = entry.replace(subContent, newValue);
            }
		}
        newData.append(newEntry);
    }
	Util::saveFileLines(newData, outputFile, false);
}

/**
 * Convert template type to the right filename.
 * @param  expType [description]
 * @return         [description]
 */
QString Export::getExportType(QString &expType) {
	QMap<QString, QString> exports;
	exports["css"] = "colors.css";
	exports["dmenu"] = "colors-wal-dmenu.h";
	exports["dwm"] = "colors-wal-dwm.h";
	exports["st"] = "colors-wal-st.h";
	exports["tabbed"] = "colors-wal-tabbed.h";
	exports["gtk2"] = "colors-gtk2.rc";
	exports["json"] = "colors.json";
	exports["konsole"] = "colors-konsole.colorscheme";
	exports["kitty"] = "colors-kitty.conf";
	exports["nqq"] = "colors-nqq.css";
	exports["plain"] = "colors";
	exports["putty"] = "colors-putty.reg";
	exports["rofi"] = "colors-rofi.Xresources";
	exports["scss"] = "colors.scss";
	exports["shell"] = "colors.sh";
	exports["speedcrunch"] = "colors-speedcrunch.json";
	exports["sway"] = "colors-sway";
	exports["tty"] = "colors-tty.sh";
	exports["vscode"] = "colors-vscode.json";
	exports["waybar"] = "colors-waybar.css";
	exports["xresources"] = "colors.Xresources";
	exports["xmonad"] = "colors.hs";
	exports["yaml"] = "colors.yml";


	return exports.contains(expType) ? exports[expType] : "None";
}