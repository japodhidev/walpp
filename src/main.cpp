#include <cstdlib>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTextStream>
#include "../include/color.h"
#include "../include/image.h"
#include "../include/settings.h"
#include "../include/util.h"
#include "../include/wallpaper.h"
#include "../include/reload.h"
#include "../include/sequences.h"

void parseExitingArgs(QCommandLineParser &parser);

void parseArgs(QCommandLineParser &parser);

int main(int argc, char *argv[]) {
    QCoreApplication coreApplication(argc, argv);
    QCoreApplication::setApplicationName("walpp");
    QCoreApplication::setApplicationVersion(Setting::version);
    QString sampleColor = "#FBAF28";
    Color color(sampleColor);
    /*qDebug() << "color -> " << color.walColor.name(QColor::HexRgb);
    qDebug() << "rgb -> " << color.rgb();
    qDebug() << "rgba -> " << color.rgba();
    qDebug() << "decimal -> " << color.decimal();
    qDebug() << "decimal strip -> " << color.decimalStrip();
    qDebug() << "xrgba -> " << color.xrgba();
    qDebug() << "octal -> " << color.octal();
    qDebug() << "octal strip -> " << color.octalStrip();
    qDebug() << "alpha -> " << color.alpha();
    qDebug() << "alpha decimal -> " << color.alphaDecimal();
    qDebug() << "strip -> " << color.strip();
    qDebug() << "red -> " << color.red();
    qDebug() << "green -> " << color.green();
    qDebug() << "blue -> " << color.blue();
    qDebug() << "lighten -> " << color.lighten(20);
    qDebug() << "darken -> " << color.darken(50);
    qDebug() << "saturate -> " << color.saturate(48);*/

    QString confDir = Setting::CONF_DIR;
    Util::createDir(Util::joinPath(confDir, QStringList() << "templates"));
    Util::createDir(Util::joinPath(confDir, QStringList() << "colorschemes" << "light"));
    Util::createDir(Util::joinPath(confDir, QStringList() << "colorschemes" << "dark"));

    // Process args
    QCommandLineParser parser;
    parser.setApplicationDescription("wal - Generate colorschemes on the fly");

    QCommandLineOption alpha(
        QStringList() << "a" << "alpha",
        QCoreApplication::translate("main", "Set terminal background transparency. *Only works in URxvt*"),
        QCoreApplication::translate("main", "alpha")
        );
    parser.addOption(alpha);

    QCommandLineOption background(
        QStringList() << "b" << "background",
        QCoreApplication::translate("main", "Custom background color to use."),
        QCoreApplication::translate("main", "background")
        );
    parser.addOption(background);

    QCommandLineOption backend(
        QStringList() << "backend" << "backend",
        QCoreApplication::translate("main", "Which color backend to use. Use 'walpp --backend' to list backends."),
        QCoreApplication::translate("main", "backend")
        );
    parser.addOption(backend);

    QCommandLineOption theme(
        QStringList() << "theme" << "theme",
        QCoreApplication::translate("main", "Which colorscheme file to use. Use 'walpp --theme' to list builtin and user themes."),
        QCoreApplication::translate("main", "theme")
        );
    parser.addOption(theme);

    QCommandLineOption iterative(
        "iterative",
        QCoreApplication::translate("main", "When walpp is given a directory as input and this flag is used: Go through the images in order instead of shuffled.")
        );
    parser.addOption(iterative);

    QCommandLineOption recursive(
        "recursive",
        QCoreApplication::translate("main", "When walpp is given a directory as input and this flag is used: Search for images recursively in subdirectories instead of the root only.")
        );
    parser.addOption(recursive);

    QCommandLineOption saturate(
        QStringList() << "saturate" << "saturate",
        QCoreApplication::translate("main", "Set the color saturation."),
        QCoreApplication::translate("main", "saturate")
        );
    parser.addOption(saturate);

    QCommandLineOption preview(
        "preview",
        QCoreApplication::translate("main", "Print the current color palette.")
        );
    parser.addOption(preview);

    QCommandLineOption vte(
        "vte",
        QCoreApplication::translate("main", "Fix text-artifacts printed in VTE terminals.")
        );
    parser.addOption(vte);

    QCommandLineOption deleteCached(
        "c",
        QCoreApplication::translate("main", "Delete all cached colorschemes.")
        );
    parser.addOption(deleteCached);

    QCommandLineOption directory(
        QStringList() << "i" << "directory",
        QCoreApplication::translate("main", "Which image or directory to use."),
        QCoreApplication::translate("main", "directory")
        );
    parser.addOption(directory);

    QCommandLineOption light(
        "l",
        QCoreApplication::translate("main", "Generate a light colorscheme.")
        );
    parser.addOption(light);

    QCommandLineOption skipWallpaper(
        "n",
        QCoreApplication::translate("main", "Skip setting the wallpaper.")
        );
    parser.addOption(skipWallpaper);

    QCommandLineOption externalScript(
        QStringList() << "o" << "externalScript",
        QCoreApplication::translate("main", "External script to run after 'wal'."),
        QCoreApplication::translate("main", "externalScript")
        );
    parser.addOption(externalScript);

    QCommandLineOption saveTheme(
        QStringList() << "p" << "saveTheme",
        QCoreApplication::translate("main", "Permanently save theme to $XDG_CONFIG_HOME/wal/colorschemes with the specified name."),
        QCoreApplication::translate("main", "saveTheme")
        );
    parser.addOption(saveTheme);

    QCommandLineOption quietMode(
        "q",
        QCoreApplication::translate("main", "Quiet mode, don't print anything.")
        );
    parser.addOption(quietMode);

    QCommandLineOption skipTerminal(
        "s",
        QCoreApplication::translate("main", "Skip changing colors in terminals.")
        );
    parser.addOption(skipTerminal);

    QCommandLineOption skipTTY(
        "t",
        QCoreApplication::translate("main", "Skip changing colors in tty.")
        );
    parser.addOption(skipTTY);

    QCommandLineOption version(
        "v",
        QCoreApplication::translate("main", "Print walpp version.")
        );
    parser.addOption(version);

    QCommandLineOption lastWal(
        "w",
        QCoreApplication::translate("main", "Use last used wallpaper for color generation.")
        );
    parser.addOption(lastWal);

    QCommandLineOption skipMost(
        "e",
        QCoreApplication::translate("main", "Skip reloading gtk/xrdb/i3/sway/polybar")
        );
    parser.addOption(skipMost);
    // Parse provided arguments
    parser.process(coreApplication);
    // Process exiting args
    parseExitingArgs(parser);
    // Process args
    parseArgs(parser);

    return QCoreApplication::exec();
}

void parseExitingArgs(QCommandLineParser &parser) {
    Util util;
    // Process args that exit
    if (parser.optionNames().size() == 0) {
        parser.showHelp();
    }

    // qDebug() << "Test";
    // std::exit(EXIT_FAILURE);

    if (parser.isSet("preview")) {
        qDebug() << "Current colorscheme:";
        util.palette();
        std::exit(EXIT_SUCCESS);
    }

    if (parser.isSet("directory") & parser.isSet("theme")) {
        qDebug() << "Conflicting arguments -i and -f.";
        std::exit(EXIT_FAILURE);
    }

    if (parser.isSet("c")) {
        qDebug() << "delete cached schemes";
        QString schemeDir = Util::joinPath(Setting::CACHE_DIR, QStringList() << "schemes");
        qDebug() << QString("TODO: Remove %1 directory.").arg(schemeDir);
    }

    if (!parser.isSet("directory") & !parser.isSet("theme") & !parser.isSet("w") & !parser.isSet("backend")) {
        qDebug() << "Error: No input specified.";
        qDebug() << "--backend, --theme, -i or -R are required.";
        std::exit(EXIT_FAILURE);
    }

    if (parser.value("theme") == "list_themes") {
        qDebug() << "theme.list_out()";
        std::exit(EXIT_SUCCESS);
    }

    if (parser.value("backend") == "list_backends") {
        QTextStream ts(stdout);
        foreach (const auto &b, Util::listBackends()) {
            ts << "\n - " << "\033[1;32mBackends\033[0m:" << b;
        }
        std::exit(EXIT_SUCCESS);
    }
}

void parseArgs(QCommandLineParser &parser) {
    Util util;
    Color color;
    // Process args
    if (parser.isSet("q")) {
        qDebug() << "logging.getLogger().disabled = True";
        // Redirect stdout to stderr then to /dev/null
        qDebug() << "sys.stdout = sys.stderr = open(os.devnull, 'w')";
    }

    if (parser.isSet("alpha")) {
        qDebug() << "util.Color.alpha_num = alpha";
        bool ok;
        color.alphaValue = parser.value("alpha").toDouble(&ok);
    }

    QString imageFile;
    QJsonObject plainColors;
    bool it = parser.isSet("iterative");
    bool recurse = parser.isSet("recursive");
    bool l = parser.isSet("l");

    if (parser.isSet("directory")) {
        auto img = parser.value("directory");
        Image img_o(img);
        imageFile = img_o.getImage(img, const_cast<QString &>(Setting::CACHE_DIR), it, recurse);
        qDebug() << imageFile;
        QString bEnd = parser.isSet("backend") ? parser.value("backend") : "wal";
        plainColors = util.getColors(imageFile, l, bEnd, Setting::CACHE_DIR, parser.value("saturate"));
    }

    if (parser.isSet("theme")) {
        qDebug() << "colorsPlain = theme.file(theme, light)";
    }

    /*if (parser.isSet("restore")) {
        qDebug() << "colorsPlain = theme.file(os.path.join(CACHE_DIR, 'colors.json'))";
    }*/

    if (parser.isSet("w")) {
        qDebug() << "cached_wallpaper = util.read_file(os.path.join(CACHE_DIR, 'wal'))";
        qDebug() << "colorsPlain = colors.get()";
        QString wallPath = Util::joinPath(Setting::CACHE_DIR, QStringList() << "walpp");
        auto cachedWallpaper = Util::readFile(wallPath);
        QString walStr = QString(cachedWallpaper.at(0));
        plainColors = util.getColors(walStr, l, parser.value("backend"), parser.value("saturate"));
    }

    if (parser.isSet("background")) {
        qDebug() << "Set background";
        QString bg;
        if (!parser.value("background").startsWith("#")) {
            bg = parser.value("background").prepend("#");
            // TODO: Insert new bg value to nested item
        }
    }

    if (!parser.isSet("n")) {
        qDebug() << "wallpaper.change(colors_plain['wallpaper'])";
        QString wStr = plainColors.value("wallpaper").toString();
        Wallpaper wall_o;
        wall_o.change(wStr);
    }

    if (parser.isSet("saveTheme")) {
        qDebug() << "theme.save(colors_plain, skipWallpaper, light)";
    }

    Sequences seqs;
    seqs.send(plainColors, Setting::CACHE_DIR, false, true);

    // TODO: sequences.send(colors_plain, to_send=not args.s, vte_fix=args.vte)

    // TODO: if sys.stdout.isatty():
    // TODO: colors.palette()
    Util::palette();

    // TODO: export.every(colors_plain)

    if (parser.isSet("t")) {
        qDebug() << "reload.env(tty_reload=!skipTTY)";
        Reload::env(!parser.isSet("t"));
    }

    if (parser.isSet("externalScript")) {
        qDebug() << "for cmd in args.o:\n"
                    "            util.disown([cmd])";
        QStringList script = parser.value("externalScript").split(' ');
        Util::disown(const_cast<QString &>(script.at(0)), QStringList() << script.sliced(1));
    }

    if (!parser.isSet("e")) {
        qDebug() << "reload.gtk()";
        Reload::gtk();
    }
}