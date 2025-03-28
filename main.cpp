#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "color.h"

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
    qDebug() << "lighten -> " << color.lighten(20);
    qDebug() << "darken -> " << color.darken(50);
    qDebug() << "saturate -> " << color.saturate(48);

    // Process args
    QCommandLineParser commandLineParser;
    commandLineParser.setApplicationDescription("wal - Generate colorschemes on the fly");
    commandLineParser.addHelpOption();
    // commandLineParser.addVersionOption();

    QCommandLineOption alpha(
        QStringList() << "a" << "alpha",
        QCoreApplication::translate("main", "Set terminal background transparency. *Only works in URxvt*"),
        QCoreApplication::translate("main", "alpha")
        );
    commandLineParser.addOption(alpha);

    QCommandLineOption background(
        QStringList() << "b" << "background",
        QCoreApplication::translate("main", "Custom background color to use."),
        QCoreApplication::translate("main", "background")
        );
    commandLineParser.addOption(background);

    QCommandLineOption backend(
        QStringList() << "backend" << "backend",
        QCoreApplication::translate("main", "Which color backend to use. Use 'walpp --backend' to list backends."),
        QCoreApplication::translate("main", "backend")
        );
    commandLineParser.addOption(backend);

    QCommandLineOption theme(
        QStringList() << "theme" << "theme",
        QCoreApplication::translate("main", "Which colorscheme file to use. Use 'walpp --theme' to list builtin and user themes."),
        QCoreApplication::translate("main", "theme")
        );
    commandLineParser.addOption(theme);

    QCommandLineOption iterative(
        QStringList() << "iterative" << "iterative",
        QCoreApplication::translate("main", "When walpp is given a directory as input and this flag is used: Go through the images in order instead of shuffled."),
        QCoreApplication::translate("main", "iterative")
        );
    commandLineParser.addOption(iterative);

    QCommandLineOption recursive(
        QStringList() << "recursive" << "recursive",
        QCoreApplication::translate("main", "When walpp is given a directory as input and this flag is used: Search for images recursively in subdirectories instead of the root only."),
        QCoreApplication::translate("main", "recursive")
        );
    commandLineParser.addOption(recursive);

    QCommandLineOption saturate(
        QStringList() << "saturate" << "saturate",
        QCoreApplication::translate("main", "Set the color saturation."),
        QCoreApplication::translate("main", "saturate")
        );
    commandLineParser.addOption(saturate);

    QCommandLineOption preview(
        QStringList() << "preview" << "preview",
        QCoreApplication::translate("main", "Print the current color palette."),
        QCoreApplication::translate("main", "preview")
        );
    commandLineParser.addOption(preview);

    QCommandLineOption vte(
        QStringList() << "vte" << "vte",
        QCoreApplication::translate("main", "Fix text-artifacts printed in VTE terminals."),
        QCoreApplication::translate("main", "vte")
        );
    commandLineParser.addOption(vte);

    QCommandLineOption deleteCached(
        QStringList() << "c" << "cached",
        QCoreApplication::translate("main", "Delete all cached colorschemes."),
        QCoreApplication::translate("main", "cached")
        );
    commandLineParser.addOption(deleteCached);

    QCommandLineOption directory(
        QStringList() << "i" << "directory",
        QCoreApplication::translate("main", "Which image or directory to use."),
        QCoreApplication::translate("main", "directory")
        );
    commandLineParser.addOption(directory);

    QCommandLineOption light(
        QStringList() << "l" << "light",
        QCoreApplication::translate("main", "Generate a light colorscheme."),
        QCoreApplication::translate("main", "light")
        );
    commandLineParser.addOption(light);

    QCommandLineOption skipWallpaper(
        QStringList() << "n" << "skipWallpaper",
        QCoreApplication::translate("main", "Skip setting the wallpaper."),
        QCoreApplication::translate("main", "skipWallpaper")
        );
    commandLineParser.addOption(skipWallpaper);

    QCommandLineOption externalScript(
        QStringList() << "o" << "externalScript",
        QCoreApplication::translate("main", "External script to run after 'wal'."),
        QCoreApplication::translate("main", "externalScript")
        );
    commandLineParser.addOption(externalScript);

    QCommandLineOption saveTheme(
        QStringList() << "p" << "saveTheme",
        QCoreApplication::translate("main", "Permanently save theme to $XDG_CONFIG_HOME/wal/colorschemes with the specified name."),
        QCoreApplication::translate("main", "saveTheme")
        );
    commandLineParser.addOption(saveTheme);

    QCommandLineOption quietMode(
        QStringList() << "q" << "quietMode",
        QCoreApplication::translate("main", "Quiet mode, don't print anything."),
        QCoreApplication::translate("main", "quietMode")
        );
    commandLineParser.addOption(quietMode);

    QCommandLineOption restore(
        QStringList() << "r" << "restore",
        QCoreApplication::translate("main", "Restore previous colorscheme."),
        QCoreApplication::translate("main", "restore")
        );
    commandLineParser.addOption(restore);

    QCommandLineOption skipTerminal(
        QStringList() << "s" << "skipTerminal",
        QCoreApplication::translate("main", "Skip changing colors in terminals."),
        QCoreApplication::translate("main", "skipTerminal")
        );
    commandLineParser.addOption(skipTerminal);

    QCommandLineOption skipTTY(
        QStringList() << "t" << "skipTTY",
        QCoreApplication::translate("main", "Skip changing colors in tty."),
        QCoreApplication::translate("main", "skipTTY")
        );
    commandLineParser.addOption(skipTTY);

    QCommandLineOption version(
        QStringList() << "v" << "version",
        QCoreApplication::translate("main", "Print walpp version."),
        QCoreApplication::translate("main", "version")
        );
    commandLineParser.addOption(version);

    QCommandLineOption lastWal(
        QStringList() << "w" << "lastWal",
        QCoreApplication::translate("main", "Use last used wallpaper for color generation."),
        QCoreApplication::translate("main", "lastWal")
        );
    commandLineParser.addOption(lastWal);

    QCommandLineOption skipMost(
        QStringList() << "e" << "skipMost",
        QCoreApplication::translate("main", "Skip reloading gtk/xrdb/i3/sway/polybar"),
        QCoreApplication::translate("main", "skipMost")
        );
    commandLineParser.addOption(skipMost);

    // Main app function
    /**
     * util.create_dir(os.path.join(CONF_DIR, "templates"))
     * util.create_dir(os.path.join(CONF_DIR, "colorschemes/light/"))
     * util.create_dir(os.path.join(CONF_DIR, "colorschemes/dark/"))
     */

    // Parse provided arguments
    commandLineParser.process(coreApplication);

    // Process args that exit
    if (commandLineParser.optionNames().size() <= 1) {
        commandLineParser.showHelp();
    }

    if (commandLineParser.isSet("preview")) {
        qDebug() << "Current colorscheme:";
    }

    if (commandLineParser.isSet("directory") & commandLineParser.isSet("theme")) {
        qDebug() << "Conflicting arguments -i and -f.";
    }

    if (commandLineParser.isSet("restore")) {
        qDebug() << "reload.colors()";
    }

    if (commandLineParser.isSet("cached")) {
        qDebug() << "delete cached schemes";
    }

    if (!commandLineParser.isSet("directory") & !commandLineParser.isSet("theme") & !commandLineParser.isSet("restore") & !commandLineParser.isSet("lastWal") & !commandLineParser.isSet("backend")) {
        qDebug() << "No input specified.";
        qDebug() << "--backend, --theme, -i or -R are required.";
    }

    if (commandLineParser.value("theme") == "list_themes") {
        qDebug() << "theme.list_out()";
    }

    if (commandLineParser.value("backend") == "list_backends") {
        qDebug() << "List backends" ;
    }

    // Process args
    if (commandLineParser.isSet("quiet")) {
        qDebug() << "logging.getLogger().disabled = True";
        // Redirect stdout to stderr then to /dev/null
        qDebug() << "sys.stdout = sys.stderr = open(os.devnull, 'w')";
    }

    if (commandLineParser.isSet("alpha")) {
        qDebug() << "util.Color.alpha_num = alpha";
    }

    if (commandLineParser.isSet("directory")) {
        qDebug() << "imageFile = image.get(...)";
        qDebug() << "colorsPlain = colors.get(...)";
    }

    if (commandLineParser.isSet("theme")) {
        qDebug() << "colorsPlain = theme.file(theme, light)";
    }

    if (commandLineParser.isSet("restore")) {
        qDebug() << "colorsPlain = theme.file(os.path.join(CACHE_DIR, 'colors.json'))";
    }

    if (commandLineParser.isSet("lastWal")) {
        qDebug() << "cached_wallpaper = util.read_file(os.path.join(CACHE_DIR, 'wal'))";
        qDebug() << "colorsPlain = colors.get()";
    }

    if (commandLineParser.isSet("background")) {
        qDebug() << "Set background";
    }

    if (!commandLineParser.isSet("skipWallpaper")) {
        qDebug() << "wallpaper.change(colors_plain['wallpaper'])";
    }

    if (commandLineParser.isSet("saveTheme")) {
        qDebug() << "theme.save(colors_plain, skipWallpaper, light)";
    }

    // TODO: sequences.send(colors_plain, to_send=not args.s, vte_fix=args.vte)

    // TODO: if sys.stdout.isatty():
    // TODO: colors.palette()

    // TODO: export.every(colors_plain)

    if (commandLineParser.isSet("skipMost")) {
        qDebug() << "reload.env(tty_reload=!skipTTY)";
    }

    if (commandLineParser.isSet("externalScript")) {
        qDebug() << "for cmd in args.o:\n"
                    "            util.disown([cmd])";
    }

    if (!commandLineParser.isSet("skipMost")) {
        qDebug() << "reload.gtk()";
    }

    return QCoreApplication::exec();
}

