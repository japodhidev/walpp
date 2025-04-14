#include "../include/appexception.h"
#include "../include/backend.h"
#include "../include/color.h"
#include "../include/logging.h"
#include "../include/settings.h"
#include "../include/theme.h"
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QJsonDocument>
#include <QProcess>
#include <QStandardPaths>
#include <QFileInfo>
#include <filesystem>
#include <QRandomGenerator>

Util::Util() = default;

/**
 * Invoke a system command in the background, disown it and hide it's output.
 * @brief Util::disown
 * @param cmd
 */
void Util::disown(QString &cmd, QStringList &arguments) {
    QProcess qProcess;
    // Set output & error device to /dev/null
    qProcess.setStandardOutputFile(QProcess::nullDevice());
    qProcess.setStandardErrorFile(QProcess::nullDevice());
    qProcess.setProgram(cmd);
    qProcess.setArguments(arguments);
    bool started = qProcess.startDetached();

    if (!started) {
        qDebug() << "Error starting process '" << cmd << "' with arguments" << arguments;
    }
}

/**
 * Check if process is running by name.
 * @brief Util::getPID
 * @param name
 */
bool Util::getPID(QString &name) {
    QString prog = "pidof";

    if(!Util::which(prog).isEmpty()) {
        return false;
    }
    QByteArray output;
    if (Setting::OS.toLower() == "darwin") {
        output = checkOutput("pidof", QStringList() << "-s");
    } else {
        output = checkOutput("pidof", QStringList());
    }

    return output.contains(name.toStdString());
}


/**
 * Write data to a json file.
 * @param data
 * @param exportFile
 */
void Util::saveJSONFile(QJsonObject &data, QString &exportFile) {
    QFile jsonFile(exportFile);
    QFileInfo fileInfo(exportFile);

    // Create directory if it doesn't already exist
    QString path = fileInfo.absolutePath();
    createDir(path);

    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::string message = QString("Couldn't open the JSON file '%1' for writing!").arg(fileInfo.absoluteFilePath()).toStdString();
        throw AppException(message);
    }
    QJsonDocument jsonDoc;
    jsonDoc.setObject(data);
    // Write to JSON file
    jsonFile.write(jsonDoc.toJson(QJsonDocument::Indented));
    jsonFile.close();
}

/**
 * Alias to create the cache dir.
 * @param directory
 */
void Util::createDir(QString &directory) {
    QDir dir;
    // bool success = dir.mkdir(directory);
    bool success = dir.mkpath(directory);
    if (!success) {
        std::string message = QString("Couldn't create the directory: '%1'!").arg(directory).toStdString();
        throw AppException(message);
    }
}

/**
 * Delete files
 * @param directory [description]
 */
void Util::removeDirFiles(const QString& directory) {
    QFileInfo dInfo(directory);
    if (dInfo.exists()) {
        Logging::info(QString("Deleting: %1").arg(dInfo.absoluteFilePath()));
        if (dInfo.isDir()) {
            // Delete files from directory
            QDir d(dInfo.absoluteFilePath());
            if (!d.removeRecursively()) {
                Logging::error(QString("Couldn't remove the directory '%1'.").arg(d.absolutePath()));
            }
        } else {
            // Delete file
            QFile f(dInfo.absoluteFilePath());
            if (!f.remove()) {
                Logging::error(QString("Couldn't remove the file '%1'.").arg(dInfo.absoluteFilePath()));
            }
        }
    } else {
        std::string message = QString("Non-existent directory provided: '%1'!").arg(directory).toStdString();
        throw AppException(message);
    }
}

/**
 * Read data from a file and trim newlines.
 * @param inputFile
 */
QByteArray Util::readFile(QString &inputFile) {
    QFile qFile(inputFile);
    QFileInfo fileInfo(inputFile);
    if (qFile.exists()) {
        if (qFile.open(QFile::ReadOnly)) {
            auto result = qFile.readAll();
            qFile.close();
            return result;
        }
        std::string message = QString("Couldn't open the file '%1' for reading!").arg(fileInfo.absoluteFilePath()).toStdString();
        throw AppException(message);
    } else {
        std::string message = "Non-existent file requested!";
        throw AppException(message);
    }
}

/**
 * Read data from a json file.
 * @param inputFile
 */
QJsonObject Util::readJSONFile(QString &inputFile) {
    QFile jsonFile(inputFile);
    QFileInfo fileInfo(inputFile);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        std::string message = QString("Couldn't open the JSON file '%1'  for reading!").arg(fileInfo.absoluteFilePath()).toStdString();
        throw AppException(message);
    }
    // Read JSON data from file
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonFile.readAll(), &jsonError);
    if (doc.isNull()) {
        std::string message = jsonError.errorString().toStdString();
        throw AppException(message);
    }

    if (!doc.isObject()) {
        std::string message = "Incompatible JSON file. Missing object!";
        throw AppException(message);
    }

    return doc.object();
}

/**
 * Read data from a file as is, don't strip newlines or other special characters.
 * @param inputFile
 */
QString Util::readRawFile(QString &inputFile) {
    QFile qFile(inputFile);
    QFileInfo fileInfo(inputFile);
    if (qFile.exists()) {
        if (qFile.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&qFile);
            QString text = in.readAll();
            qFile.close();
            return text;
        }
        std::string message = QString("Couldn't open the file '%1'  for reading!").arg(fileInfo.absoluteFilePath()).toStdString();
        throw AppException(message);
    } else {
        std::string message = "Non-existent file requested!";
        throw AppException(message);
    }
}

QList<QString> Util::readRawFileToList(QString &inputFile) {
    QFile qFile(inputFile);
    QFileInfo fileInfo(inputFile);
    if (qFile.exists()) {
        if (qFile.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&qFile);
            QList<QString> content;
            while(!in.atEnd()) {
                content.append(in.readLine());
            }
            qFile.close();
            return content;
        }
        std::string message = QString("Couldn't open the file '%1'  for reading!").arg(fileInfo.absoluteFilePath()).toStdString();
        throw AppException(message);
    } else {
        std::string message = "Non-existent file requested!";
        throw AppException(message);
    }
}

/**
 * Write data to a file.
 * @param data
 * @param exportFile
 */
void Util::saveFile(QString &data, QString &exportFile, bool mkDir) {
    QFile file;
    QFileInfo dataInfo(data);
    if (mkDir) {
        createDir(exportFile);
        file.setFileName(Util::joinPath(exportFile, QStringList() << dataInfo.fileName()));
    } else {
        file.setFileName(exportFile);
    }
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        // Read file contents, maybe
        QString content = (dataInfo.isFile() & mkDir) ? readRawFile(data) : data;
        // Write data to file
        QTextStream out(&file);
        out << content;
        file.close();
    } else {
        std::string message = QString("Couldn't open the file '%1' for writing!").arg(file.fileName()).toStdString();
        throw AppException(message);
    }
}

/**
 * Write data to a file, line by line.
 * @param data
 * @param exportFile
 */
void Util::saveFileLines(QList<QString> &data, QString &exportFile, bool mkDir) {
    QFile file;
    QFileInfo fileInfo(exportFile);
    if (mkDir) {
        QString path = fileInfo.absolutePath();
        createDir(path);
        file.setFileName(Util::joinPath(exportFile, QStringList() << fileInfo.fileName()));
    } else {
        file.setFileName(exportFile);
    }
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        // Write data to file
        QTextStream out(&file);
        foreach (const QString &entry, data) {
            out << entry << Qt::endl;
        }
        file.close();
    } else {
        std::string message = QString("Couldn't open the file '%1' for writing!").arg(file.fileName()).toStdString();
        throw AppException(message);
    }
}

/**
 * Append a file/directory to a path & return the new path
 * @brief Util::joinPath
 * @param oPath
 * @param addition
 * @return
 */
 QString Util::joinPath(QString oPath, QStringList addition) {
    std::filesystem::path o_p = oPath.toStdString();
    // o_p /= addition.toStdString();
    foreach (const QString entry, addition) {
        o_p /= entry.toStdString();
    }

    return QString::fromStdString(o_p.string());
}

/**
 * An implementation mimicking Python's shutil.which(). Returns the absolute path of the executable or en empty string.
 * @brief Util::which
 * @param program
 * @return
 */
QString Util::which(const QString& program) {
    return QStandardPaths::findExecutable(program);
}

/**
 * Execute a command/run a program
 * @brief Util::pOpen
 * @param command
 * @param args
 */
void Util::pOpen(const QString& command, const QStringList& args) {
    QProcess process;
    process.setStandardOutputFile(QProcess::nullDevice());
    process.setStandardErrorFile(QProcess::nullDevice());
    process.setStandardInputFile(QProcess::nullDevice());
    process.setProgram(command);
    process.setArguments(args);
    process.start();
}

/**
 * Run a command and wait for it to complete.
 * @brief Util::run
 * @param command
 * @param args
 */
void Util::run(const QString& command, QStringList &args) {
    QProcess process;
    process.setStandardOutputFile(QProcess::nullDevice());
    process.setStandardErrorFile(QProcess::nullDevice());
    process.setStandardInputFile(QProcess::nullDevice());
    process.setProgram(command);
    process.setArguments(args);
    process.waitForFinished();
}

/**
 * Start a process and return the output
 * @brief Util::checkOutput
 * @param command
 * @param arguments
 * @return
 */
QByteArray Util::checkOutput(const QString& command, const QStringList& arguments) {
    QProcess process;
    process.setStandardErrorFile(QProcess::nullDevice());
    process.setStandardInputFile(QProcess::nullDevice());
    process.start(command, arguments);
    process.waitForFinished(-1);

    QByteArray output = process.readAllStandardOutput();

    return output;
}

QList<QString> Util::listBackends() {
    QList<QString> backends = {"wal"};

    return backends;
}

/**
 * Normalizes the image path for output
 * @brief Util::normalizeImgPath
 * @param img
 * @return
 */
QString Util::normalizeImgPath(QString &img) {
    if (Setting::OS.toLower() == "windows") {
        return img.replace("\\", "/");
    }

    return img;
}

QJsonObject Util::colorsToMap(const QList<QString>& colors, QString &img) {
    QString imgPath = normalizeImgPath(img);
    QJsonObject special {
        {"background", colors.at(0)},
        {"foreground", colors.at(15)},
        {"cursor", colors.at(15)}
    };
    QJsonObject c {
        {"color0", colors.at(0)},
        {"color1", colors.at(1)},
        {"color2", colors.at(2)},
        {"color3", colors.at(3)},
        {"color4", colors.at(4)},
        {"color5", colors.at(5)},
        {"color6", colors.at(6)},
        {"color7", colors.at(7)},
        {"color8", colors.at(8)},
        {"color9", colors.at(9)},
        {"color10", colors.at(10)},
        {"color11", colors.at(11)},
        {"color12", colors.at(12)},
        {"color13", colors.at(13)},
        {"color14", colors.at(14)},
        {"color15", colors.at(15)},
    };
    Color cl;
    QJsonObject result {
        {"wallpaper", QJsonValue(imgPath)},
        {"alpha", cl.alphaValue},
        {"special", special},
        {"colors", c}
    };

    return result;
}

QList<QString> Util::genericAdjust(QList<QString> colors, bool light) {
    QString firstColor = colors.at(0);
    Color c0(firstColor);
    if (light) {
        foreach (QString color, colors) {
            Color c(color);
            color = c.saturate(0.6);
            color = c.darken(0.5);
        }

        colors.replace(0, c0.darken(0.95));
        colors.replace(7, c0.darken(0.75));
        colors.replace(8, c0.darken(0.25));
        colors.replace(15, colors.at(7));
    } else {
        colors.replace(0, c0.darken(0.80));
        colors.replace(7, c0.darken(0.75));
        colors.replace(8, c0.darken(0.25));
        colors.replace(15, colors.at(7));
    }

    return colors;
}

/**
 * Saturate colors
 * @brief Util::saturateColors
 * @param colors
 * @param amount
 * @return
 */
QList<QString> Util::saturateColors(QList<QString> colors, int amount) {
    QList<QString> newColors;
    if (amount & ((float) amount <= 1.0)) {
        newColors = Color::saturateMultiple(colors, (float) amount);
    }

    return newColors;
}

/**
 * Create the cache file name
 * @brief Util::cacheFileName
 * @param img
 * @param backend
 * @param light
 * @param cacheDir
 * @param sat
 * @return
 */
QList<QString> Util::cacheFileName(QString &img, QString &backend, bool light, QString &cacheDir, QString sat) {
    QString colorType = light ? "light" : "dark";
    QFileInfo file(img);
    QString fileName = file.absoluteFilePath().replace("/", "_").replace("\\", "_").replace(".", "_");
    auto fileSize = file.size();
    QString cacheName = QString("%1_%2_%3_%4_%5_%6.json").arg(fileName).arg(colorType).arg(backend).arg(sat).arg(fileSize).arg(Setting::cacheVersion);
    QList<QString> result = {cacheDir, "schemes", cacheName};

    return result;
}

/**
 * Figure out which backend to use.
 * @brief Util::getBackend
 * @param backend
 * @return
 */
QString Util::getBackend(QString &backend) {
    if (backend == "random") {
        QList<QString> backends = listBackends();
        auto randomIndex = QRandomGenerator(nullptr, backends.size()).generate();

        return backends.at(randomIndex);
    }
    
    return backend;
}

/**
 * Generate a palette from the colors.
 * @brief Util::palette
 */
void Util::palette() {
    QTextStream ts(stdout);
    for (int i = 0; i < 16; i++) {
        QString str_i;
        if (i % 8 == 0) {
            ts << Qt::endl;
        }

        if (i > 7) {
            str_i = QString("8;5;%1").arg(i);
        } else {
            str_i = QString("%1").arg(i);
        }

        QString spaces = QString(" ").repeated(80 / 20);
        ts << QString("\033[4%1m%2\033[0m").arg(str_i, spaces);
    }
    ts << Qt::endl << Qt::endl;
}

/**
 * Generate a palette
 * @brief Util::get
 * @param img
 * @param light
 * @param backend
 * @param cacheDir
 */
QJsonObject Util::getColors(QString img, bool light, QString backend, QString cacheDir,  QString sat) {
    QTextStream out(stdout);
    QList<QString> cacheName = cacheFileName(img, backend, light, cacheDir, sat);
    QString cacheFile = joinPath(cacheName.at(0), QStringList() << cacheName.at(1) << cacheName.at(2));
    QFileInfo cFile(cacheFile);
    QJsonObject cs;
    if (cFile.isFile()) {
        Logging::info(QString("\033[1;31m%1\033[0m: %2").arg("colors", "Found cached colorscheme."));
        Theme th;
        Color color;
        QString cFilePath = cFile.absoluteFilePath();
        cs = th.import(cFilePath);
        auto alpha = QJsonValue(color.alphaValue);
        cs.insert("alpha",  alpha);
    } else {
        Logging::info(QString("\033[1;31m%1\033[0m: %2").arg("colors", "Generating a colorscheme."));
        QString bEnd = getBackend(backend);
        Logging::info(QString("\033[1;31m%1\033[0m: Using %2 backend.").arg("colors", bEnd));
        if (bEnd == "wal") {
            QList<QString> colorList = Wal::get(img, light);
            bool ok;
            QString saturation = sat.isEmpty() ? "0" : sat;
            // Only saturate colors 1,2,3,4,5,6,9,10,11,12,13,14
            for (int i = 0; i < colorList.size(); i++) {
                if (i == 1 | i == 2 | i == 3 | i == 4 | i == 5 | i == 6 | i == 9 | i == 10 | i == 11 | i == 12 | i == 13 | i == 14){
                    // Saturate color
                    colorList.replace(i, Color::c_saturate(saturation.toFloat(&ok), colorList.at(i)));
                }
            }
            cs  = colorsToMap(colorList, img);
            Util util;
            QString fPath = cFile.absoluteFilePath();
            util.saveJSONFile(cs, fPath);
            Logging::info(QString("\033[1;31m%1\033[0m: %2").arg("colors", "Generation complete."));
        } else {
            qDebug() << QString("Unsupported backend - %1").arg(backend);
        }
    }

    return cs;
}

int Util::getRandomInt(int min, int max) {
    QRandomGenerator rd;
    return rd.bounded(min, max);
}
