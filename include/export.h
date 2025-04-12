#ifndef EXPORT_H
#define EXPORT_H

#include "color.h"
#include "settings.h"
#include "util.h"
#include <QJsonObject>
#include <QMap>
#include <QRegularExpression>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QTextStream>


class Export {
public:
	Export();
	static void exportAll(QJsonObject &colors, const QString& outputDir = Setting::CACHE_DIR);
	static void exportSingle(QJsonObject &colors, QString &exportType, const QString& outputFile = nullptr);
private:
	static QMap<QString, QString> flattenColors(QJsonObject &colors);
	static void parseTemplate(QMap<QString, QString> &colors, QString &inputFile, QString outputFile = nullptr);
	static QString getExportType(QString &xpType);
	
};

#endif // EXPORT_H