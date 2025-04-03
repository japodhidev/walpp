#include <cstdlib>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "../include/color.h"
#include "../include/image.h"
#include "../include/settings.h"
#include "../include/util.h"


int main(int argc, char *argv[]) {
    QCoreApplication coreApplication(argc, argv);
    QCoreApplication::setApplicationName("walpp");
    QCoreApplication::setApplicationVersion("0.1");
    QString sampleColor = "#FBAF28";
    Color color(sampleColor);
    qDebug() << "color -> " << color.walColor.name(QColor::HexRgb);
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
    qDebug() << "lighten -> " << Color::lighten(20);
    qDebug() << "darken -> " << Color::darken(50);
    qDebug() << "saturate -> " << Color::saturate(48);

    // Process args
    QCommandLineParser parser;
    parser.setApplicationDescription("wal - Generate colorschemes on the fly");
    parser.addHelpOption();
    // parser.addVersionOption();

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
        QStringList() << "iterative" << "iterative",
        QCoreApplication::translate("main", "When walpp is given a directory as input and this flag is used: Go through the images in order instead of shuffled."),
        QCoreApplication::translate("main", "iterative")
        );
    parser.addOption(iterative);

    QCommandLineOption recursive(
        QStringList() << "recursive" << "recursive",
        QCoreApplication::translate("main", "When walpp is given a directory as input and this flag is used: Search for images recursively in subdirectories instead of the root only."),
        QCoreApplication::translate("main", "recursive")
        );
    parser.addOption(recursive);

    QCommandLineOption saturate(
        QStringList() << "saturate" << "saturate",
        QCoreApplication::translate("main", "Set the color saturation."),
        QCoreApplication::translate("main", "saturate")
        );
    parser.addOption(saturate);

    QCommandLineOption preview(
        QStringList() << "preview" << "preview",
        QCoreApplication::translate("main", "Print the current color palette."),
        QCoreApplication::translate("main", "preview")
        );
    parser.addOption(preview);

    QCommandLineOption vte(
        QStringList() << "vte" << "vte",
        QCoreApplication::translate("main", "Fix text-artifacts printed in VTE terminals."),
        QCoreApplication::translate("main", "vte")
        );
    parser.addOption(vte);

    QCommandLineOption deleteCached(
        QStringList() << "c" << "cached",
        QCoreApplication::translate("main", "Delete all cached colorschemes."),
        QCoreApplication::translate("main", "cached")
        );
    parser.addOption(deleteCached);

    QCommandLineOption directory(
        QStringList() << "i" << "directory",
        QCoreApplication::translate("main", "Which image or directory to use."),
        QCoreApplication::translate("main", "directory")
        );
    parser.addOption(directory);

    QCommandLineOption light(
        QStringList() << "l" << "light",
        QCoreApplication::translate("main", "Generate a light colorscheme."),
        QCoreApplication::translate("main", "light")
        );
    parser.addOption(light);

    QCommandLineOption skipWallpaper(
        QStringList() << "n" << "skipWallpaper",
        QCoreApplication::translate("main", "Skip setting the wallpaper."),
        QCoreApplication::translate("main", "skipWallpaper")
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
        QStringList() << "q" << "quietMode",
        QCoreApplication::translate("main", "Quiet mode, don't print anything."),
        QCoreApplication::translate("main", "quietMode")
        );
    parser.addOption(quietMode);

    // QCommandLineOption restore(
    //     QStringList() << "r" << "restore",
    //     QCoreApplication::translate("main", "Restore previous colorscheme."),
    //     QCoreApplication::translate("main", "restore")
    //     );
    // parser.addOption(restore);

    QCommandLineOption skipTerminal(
        QStringList() << "s" << "skipTerminal",
        QCoreApplication::translate("main", "Skip changing colors in terminals."),
        QCoreApplication::translate("main", "skipTerminal")
        );
    parser.addOption(skipTerminal);

    QCommandLineOption skipTTY(
        QStringList() << "t" << "skipTTY",
        QCoreApplication::translate("main", "Skip changing colors in tty."),
        QCoreApplication::translate("main", "skipTTY")
        );
    parser.addOption(skipTTY);

    QCommandLineOption version(
        QStringList() << "v" << "version",
        QCoreApplication::translate("main", "Print walpp version."),
        QCoreApplication::translate("main", "version")
        );
    parser.addOption(version);

    QCommandLineOption lastWal(
        QStringList() << "w" << "lastWal",
        QCoreApplication::translate("main", "Use last used wallpaper for color generation."),
        QCoreApplication::translate("main", "lastWal")
        );
    parser.addOption(lastWal);

    QCommandLineOption skipMost(
        QStringList() << "e" << "skipMost",
        QCoreApplication::translate("main", "Skip reloading gtk/xrdb/i3/sway/polybar"),
        QCoreApplication::translate("main", "skipMost")
        );
    parser.addOption(skipMost);

    // Main app function
    /**
     * util.create_dir(os.path.join(CONF_DIR, "templates"))
     * util.create_dir(os.path.join(CONF_DIR, "colorschemes/light/"))
     * util.create_dir(os.path.join(CONF_DIR, "colorschemes/dark/"))
     */

    // Parse provided arguments
    parser.process(coreApplication);
    Util util;

    // Process args that exit
    if (parser.optionNames().size() <= 1) {
        parser.showHelp();
    }

    if (parser.isSet("preview")) {
        qDebug() << "Current colorscheme:";
        util.palette();
        std::exit(EXIT_SUCCESS);
    }

    if (parser.isSet("directory") & parser.isSet("theme")) {
        qDebug() << "Conflicting arguments -i and -f.";
        std::exit(EXIT_FAILURE);
    }

    // if (parser.isSet("restore")) {
    //     qDebug() << "reload.colors()";
    // }

    if (parser.isSet("cached")) {
        qDebug() << "delete cached schemes";
        QString schemeDir = Util::joinPath(Setting::CACHE_DIR, QStringList() << "schemes");
        qDebug() << QString("TODO: Remove %1 directory.").arg(schemeDir);
    }

    if (!parser.isSet("directory") & !parser.isSet("theme") & !parser.isSet("restore") & !parser.isSet("lastWal") & !parser.isSet("backend")) {
        qDebug() << "No input specified.";
        qDebug() << "--backend, --theme, -i or -R are required.";
    }

    if (parser.value("theme") == "list_themes") {
        qDebug() << "theme.list_out()";
    }

    if (parser.value("backend") == "list_backends") {
        qDebug() << "List backends" ;
    }

    // Process args
    if (parser.isSet("quiet")) {
        qDebug() << "logging.getLogger().disabled = True";
        // Redirect stdout to stderr then to /dev/null
        qDebug() << "sys.stdout = sys.stderr = open(os.devnull, 'w')";
    }

    if (parser.isSet("alpha")) {
        qDebug() << "util.Color.alpha_num = alpha";
        bool *ok = reinterpret_cast<bool *>(true);
        color.alphaValue = parser.value("alpha").toDouble(ok);
    }
    QString imageFile;
    QJsonObject plainColors;
    bool it = !parser.value("iterative").isEmpty();
    bool recurse = !parser.value("recursive").isEmpty();
    bool l = !parser.value("light").isEmpty();
    QString dir = parser.value("directory");

    if (parser.isSet("directory")) {
        qDebug() << "imageFile = image.get(...)";
        qDebug() << "colorsPlain = colors.get(...)";
        auto img = parser.value("directory");
        Image img_o(img);
        imageFile = img_o.getImage(dir, const_cast<QString &>(Setting::CACHE_DIR), it, recurse);
        plainColors = util.getColors(img, l, parser.value("backend"), Setting::CACHE_DIR, parser.value("saturate"));
    }

    if (parser.isSet("theme")) {
        qDebug() << "colorsPlain = theme.file(theme, light)";
    }

    if (parser.isSet("restore")) {
        qDebug() << "colorsPlain = theme.file(os.path.join(CACHE_DIR, 'colors.json'))";
    }

    if (parser.isSet("lastWal")) {
        qDebug() << "cached_wallpaper = util.read_file(os.path.join(CACHE_DIR, 'wal'))";
        qDebug() << "colorsPlain = colors.get()";
        QString wallPath = Util::joinPath(Setting::CACHE_DIR, QStringList() << "walpp");
        auto cachedWallpaper = util.readFile(wallPath);
        QString walStr = QString(cachedWallpaper.at(0));
        plainColors = util.getColors(walStr, l, parser.value("backend"), parser.value("saturate"));
    }

    if (parser.isSet("background")) {
        qDebug() << "Set background";
        if (parser.value("background").startsWith("#")) {}
    }

    if (!parser.isSet("skipWallpaper")) {
        qDebug() << "wallpaper.change(colors_plain['wallpaper'])";
    }

    if (parser.isSet("saveTheme")) {
        qDebug() << "theme.save(colors_plain, skipWallpaper, light)";
    }

    // TODO: sequences.send(colors_plain, to_send=not args.s, vte_fix=args.vte)

    // TODO: if sys.stdout.isatty():
    // TODO: colors.palette()

    // TODO: export.every(colors_plain)

    if (parser.isSet("skipMost")) {
        qDebug() << "reload.env(tty_reload=!skipTTY)";
    }

    if (parser.isSet("externalScript")) {
        qDebug() << "for cmd in args.o:\n"
                    "            util.disown([cmd])";
    }

    if (!parser.isSet("skipMost")) {
        qDebug() << "reload.gtk()";
    }

    return QCoreApplication::exec();
}

