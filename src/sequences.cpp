#include "../include/logging.h"
#include "../include/sequences.h"
#include "../include/util.h"
#include <QDirIterator>
#include <QRegularExpression>
#include <QDirListing>

Sequences::Sequences() = default;

/**
 * Sequences::send Send colors to all open terminals
 * @param colors   [description]
 * @param cacheDir [description]
 * @param toSend   [description]
 * @param vteFix   [description]
 */
void Sequences::send(QJsonObject &colors, QString cacheDir, bool toSend, bool vteFix) {
    auto sequences = createSequences(colors, vteFix);

    // Writing to "/dev/pts/[0-9] lets you send data to open terminals.
    if (toSend) {
        // Find & write to all terms
        QList<QString> terminals = findTerminals();

        foreach (QString terminal, terminals) {
            Util::saveFile(sequences, terminal, false);
        }
    }
    QString seqPath = Util::joinPath(cacheDir, QStringList() << "sequences");
    Util::saveFile(sequences, seqPath, false);
    Logging::info("Set terminal colors");
}

/**
 * Sequences::setSpecial Convert a hex color to a special sequence
 * @param  index     [description]
 * @param  color     [description]
 * @param  itermName [description]
 * @param  alpha     [description]
 * @return           [description]
 */
QString Sequences::setSpecial(int index, QString color, const QString& itermName, int alpha) {
    if (Setting::OS.toLower() == "darwin" & !itermName.isEmpty()) {
        return QString("\033]P%1%2\033\\").arg(itermName, color.remove('#'));
    }

    if (index >= 11 & index <= 708 & alpha != 100) {
        return QString("\033]%1;[%2]%3\033\\").arg(index).arg(alpha).arg(color);
    }

    return QString("\033]%1;%2\033\\").arg(index).arg(color);
}

/**
 * Sequences::setColor Convert a hex color to a text color sequence
 * @param  index    [description]
 * @param  hexColor [description]
 * @return          [description]
 */
QString Sequences::setColor(int index, QString &hexColor) {
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
QString Sequences::setItermTabColor(rgb_t &color) {
    return {};
}

/**
 * [Sequences::createSequences Create the escape sequences]
 * @param  colors [description]
 * @param  vteFix [description]
 * @return        [description]
 */
QString Sequences::createSequences(QJsonObject &colors, bool vteFix) {
    QString alpha = QString("%1").arg(colors.value("alpha").toInt());
    QList<QString> seqs;

    for (int i = 0; i <= 16; i++) {
        QString cName = QString("color%1").arg(i);
        QJsonObject c = colors.value("colors").toObject();
        QString cStr = c.value(cName).toString();
        QString temp = setColor(i, cStr);
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
    QString bg = special.value("background").toString();
    QString fg  = special.value("foreground").toString();
    QString cursor = special.value("background").toString();
    seqs << setColor(232, bg);
    seqs << setColor(256, fg);
    seqs << setColor(257, cursor);

    if (vteFix) {
        seqs << setSpecial(708, special.value("background").toString(), "", alpha.toInt(&ok));
    }

    if (Setting::OS.toLower() == "darwin") {
        // FIXME sequences += set_iterm_tab_color(colors["special"]["background"])
    }

    return seqs.join("");
}

/**
 * Search for a list of terminals at the specified directory
 * @return
 */
QList<QString> Sequences::findTerminals() {
    QDirIterator iter("/dev", QDirIterator::Subdirectories);
    QString ttyDirPattern;
    QRegularExpression regex;
    QList<QString> terminals;

    if (Setting::OS.toLower() == "darwin") {
        ttyDirPattern = "/dev/ttys00[0-9]*";
    } else {
        ttyDirPattern = "/dev/pts/[0-9]+";
    };

    regex.setPattern(ttyDirPattern);

    QString devDir = "/dev";
    using ItFlag = QDirListing::IteratorFlag;
    for (const auto &dirEntry : QDirListing(devDir, ItFlag::Recursive)) {
        QString path = dirEntry.filePath();
        auto match = regex.match(path);

        if (match.hasMatch()) {
            terminals.append(match.captured(0));
        }
    }

    return terminals;
}
