#include "../include/sequences.h"
#include "../include/settings.h"
#include "../include/util.h"
#include <QDir>
#include <QRegularExpression>
#include <QFileInfoList>

Sequences::Sequences() = default;

/**
 * [Sequences::send Send colors to all open terminals]
 * @param colors   [description]
 * @param cacheDir [description]
 * @param toSend   [description]
 * @param vteFix   [description]
 */
void Sequences::send(QJsonObject colors, QString cacheDir, bool toSend, bool vteFix) {
    QString ttyPatern;
    if (Setting::OS.toLower() == "darwin") {
        ttyPatern = "/dev/ttys00[0-9]*";
    } else {
        ttyPatern = "/dev/pts/[0-9]*";
    }

    auto sequences = createSequences(colors, vteFix);

    // Writing to "/dev/pts/[0-9] lets you send data to open terminals.
    if (toSend) {
        // Find & write to all terms
        QList<QString> terminals = findTerminals();

        for (QString terminal : terminals) {
            Util::saveFile(sequences, terminal);
        }
    }
    QString seqPath = Util::joinPath(cacheDir, QStringList() << "sequences");
    Util::saveFile(sequences, seqPath);
    qDebug() << "Set terminal colors";
}

/**
 * [Sequences::setSpecial Convert a hex color to a special sequence]
 * @param  index     [description]
 * @param  color     [description]
 * @param  itermName [description]
 * @param  alpha     [description]
 * @return           [description]
 */
QString Sequences::setSpecial(int index, QString color, QString itermName, int alpha) {
    if (Setting::OS.toLower() == "darwin" & !itermName.isEmpty()) {
        return QString("\033]P%1%2\033\\").arg(itermName, color.remove('#'));
    }

    if (index >= 11 & index <= 708 & alpha != 100) {
        return QString("\033]%1;[%2]%3\033\\").arg(index).arg(alpha).arg(color);
    }

    return QString("\033]%1;%2\033\\").arg(index).arg(color);
}

/**
 * [Sequences::setColor Convert a hex color to a text color sequence]
 * @param  index    [description]
 * @param  hexColor [description]
 * @return          [description]
 */
QString Sequences::setColor(int index, QString hexColor) {
    if (Setting::OS.toLower() == "darwin") {
        return QString("\033]P%1x%2\033\\").arg(index).arg(hexColor);
    }

    return QString("\033]4;%1;%2\033\\").arg(index).arg(hexColor);
}

/**
 * FIXME
 * [Sequences::setItermTabColor Set iTerm tab/window color]
 * @param  color [description]
 * @return       [description]
 */
QString Sequences::setItermTabColor(rgb_t color) {
    return {};
}

/**
 * [Sequences::createSequences Create the escape sequences]
 * @param  colors [description]
 * @param  vteFix [description]
 * @return        [description]
 */
QString Sequences::createSequences(QJsonObject colors, bool vteFix) {
    QString alpha = colors.value("alpha").toString();
    QList<QString> seqs;

    for (int i = 0; i <= 16; i++) {
        QString cName = QString("color%1").arg(i);
        QJsonObject c = colors.value("colors").toObject();
        QString temp = setColor(i, c.value(cName).toString());
        seqs.append(temp);
    }

    /*Special colors
    Source: https://goo.gl/KcoQgP
    10 = foreground, 11 = background, 12 = cursor foreground
    13 = mouse foreground, 708 = background border color.*/
    bool ok;
    QJsonObject special = colors.value("special").toObject();
    seqs << setSpecial(10, special.value("foreground").toString(), "g");
    seqs << setSpecial(11, special.value("background").toString(), "h", alpha.toInt(&ok));
    seqs << setSpecial(12, special.value("cursor").toString(), "l");
    seqs << setSpecial(13, special.value("foreground").toString(), "j");
    seqs << setSpecial(17, special.value("foreground").toString(), "k");
    seqs << setSpecial(19, special.value("background").toString(), "m");
    seqs << setColor(232, special.value("background").toString());
    seqs << setColor(256, special.value("foreground").toString());
    seqs << setColor(257, special.value("background").toString());

    if (vteFix) {
        seqs << setSpecial(708, special.value("background").toString(), "", alpha.toInt(&ok));
    }

    if (Setting::OS.toLower() == "darwin") {
        // FIXME sequences += set_iterm_tab_color(colors["special"]["background"])
    }

    return {};
}

QList<QString> Sequences::findTerminals() {
    QDir dir("/dev");
    QStringList terminalPatterns = {"/dev/pts/[0-9]*", "/dev/ttys00[0-9]*"};
    QRegularExpression regex;
    QList<QString> terminals;

    for (const QString &pattern : terminalPatterns) {
        regex.setPattern(QRegularExpression::escape(pattern));
        QFileInfoList files = dir.entryInfoList(QDir::Files);

        for (const QFileInfo &fileInfo : files) {
            if (regex.match(fileInfo.fileName()).hasMatch()) {
                terminals.append(fileInfo.fileName());
            }
        }
    }

    return terminals;
}
