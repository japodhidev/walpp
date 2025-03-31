#include "util.h"
#include "appexception.h"
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QJsonDocument>
#include <QProcess>
#include <filesystem>

Util::Util() {}

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
    qProcess.setArguments(arguments);
    qProcess.startDetached(cmd);
}

/**
 * Check if process is running by name.
 * @brief Util::getPID
 * @param name
 */
bool Util::getPID(QString &name) {
    return false;
}


/**
 * Write data to a json file.
 * @param data
 * @param exportFile
 */
void Util::saveJSONFile(QJsonObject &data, QString &exportFile) {
    QFile jsonFile(exportFile);

    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::string message = "Couldn't open the JSON file for writing!";
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
    bool success = dir.mkdir(directory);
    if (!success) {
        std::string message = "Couldn't create the directory specified!";
        throw AppException(message);
    }
}

/**
 * Logging config.
 */
void Util::setupLogging() {

}

/**
 * Read data from a file and trim newlines.
 * @param inputFile
 */
QByteArray Util::readFile(QString &inputFile) {
    QFile qFile(inputFile);
    if (qFile.exists()) {
        if (qFile.open(QFile::ReadOnly)) {
            auto result = qFile.readAll();
            qFile.close();
            return result;
        }
        std::string message = "Couldn't open the file for reading!";
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
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        std::string message = "Couldn't open the JSON file for reading!";
        throw AppException(message);
    }
    // Read JSON data from file
    QJsonParseError jsonError;
    QJsonDocument doc;
    doc.fromJson(jsonFile.readAll(), &jsonError);
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
    if (qFile.exists()) {
        if (qFile.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&qFile);
            QString text = in.readAll();
            qFile.close();
            return text;
        }
        std::string message = "Couldn't open the file for reading!";
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
void Util::saveFile(QString &data, QString &exportFile) {
    QFile file;
    QDir::setCurrent("/tmp");
    file.setFileName(exportFile);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        // Write data to file
        auto content = QByteArray::fromStdString(data.toStdString());
        file.write(content);
        file.close();
    } else {
        std::string message = "Couldn't open the file for writing!";
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
QString Util::joinPath(QString oPath, QString addition) {
    std::filesystem::path o_p = oPath.toStdString();
    o_p /= addition.toStdString();

    return QString::fromStdString(o_p.string());
}
