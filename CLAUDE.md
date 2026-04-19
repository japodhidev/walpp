# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

`walpp` is a C++/Qt rewrite of most of [pywal](https://github.com/dylanaraps/pywal). It generates a terminal/desktop colorscheme from a wallpaper image, applies it (wallpaper, terminal escape sequences, GTK/xrdb/i3/sway/polybar reloads), and exports templated color files. Linux-only at present.

## Build

- Toolchain: CMake ≥ 3.19, Ninja, Qt6 ≥ 6.5 (`Core`, `Gui`), ImageMagick 7 Magick++ (headers at `/usr/include/ImageMagick-7/`, with `MAGICKCORE_QUANTUM_DEPTH=16` and `MAGICKCORE_HDRI_ENABLE=0`), libcurl, C++20.
- `build.sh` is a convenience wrapper that configures into `cmake-build-debug/` using CLion's bundled Ninja (`/opt/clion/bin/ninja/linux/x64/ninja`). For a portable build, invoke CMake directly, e.g. `cmake -G Ninja -S . -B build && cmake --build build --target walpp`.
- There is no test suite and no lint configuration — don't invent commands for them.

## Architecture

`src/main.cpp` is the only entry point. It drives everything through `QCoreApplication` + `QCommandLineParser`, splitting into two phases: `parseExitingArgs` (flags that print and exit: `--preview`, `-c`, `--theme list_themes`, `--backend list_backends`, conflicting flag checks) and `parseArgs` (the full pipeline). The CLI surface intentionally mirrors pywal's (`-a`, `-b`, `--backend`, `--theme`, `-i`, `-l`, `-n`, `-o`, `-p`, `-s`, `-t`, `-w`, `-e`, `--saturate`, `--vte`, `--iterative`, `--recursive`).

The pipeline in `parseArgs` runs in this fixed order and modules communicate via a single `QJsonObject plainColors` (schema: `wallpaper`, `alpha`, `special.{background,foreground,cursor}`, `colors.color0..color15`):

1. **Image selection** — `Walpp::Image` (`image.{h,cpp}`) picks a file from a path or directory (random / iterative / recursive, `png|jpg|jpeg|jpe|gif|webp`). Magick++ is used for pixel access.
2. **Color extraction** — `Util::getColors` (`util.{h,cpp}`) dispatches to a backend by name. Backends currently implemented: `wal` (`backend.{h,cpp}`, shells out to ImageMagick and adjusts via `Color::genericAdjust`) and `haishoku` (`haishoku.{h,cpp}` + `HaishokuAlgo.{h,cpp}` + `HaishokuImage.{h,cpp}`, a direct C++ port of the Python haishoku package — sort-by-RGB, bucket into a 3×3×3 accuracy grid, weighted mean per group). Results are cached under `$XDG_CACHE_HOME/wal/schemes/` and keyed by `cacheFileName(img, backend, light, sat)`.
3. **Color ops** — `Color` (`color.{h,cpp}`) wraps `QColor` and provides `rgb/rgba/xrgba/alpha/decimal/octal/strip/lighten/darken/saturate/blendColor` plus YIQ conversion, used by backends for the 16-color palette adjustment and by `sequences`/`export`.
4. **Wallpaper** — `Wallpaper::change` (`wallpaper.{h,cpp}`) detects the desktop environment and sets the wallpaper (xfconf, WM-specific, macOS/Windows stubs). Skipped when `-n` is set.
5. **Theme save** — `Theme::save` (`theme.{h,cpp}`) writes the palette to `$XDG_CONFIG_HOME/wal/colorschemes/{dark,light}/<name>.json`. `Theme::import` / `listAllThemes` / `terminalSexyToWal` handle the reverse.
6. **Terminal sequences** — `Sequences::send` (`sequences.{h,cpp}`) writes OSC escape sequences to each pty under `/dev/pts/*` and caches them to `CACHE_DIR/sequences`. Respects `--vte` and `-s`.
7. **Palette preview + export** — `Util::palette()` prints a color grid. `Export::exportAll` (`export.{h,cpp}`) walks `$XDG_CONFIG_HOME/wal/templates/`, substitutes tokens via `flattenColors` / `parseTemplate`, and writes to `CACHE_DIR`.
8. **Reload** — `Reload::env` then `Reload::gtk` (`reload.{h,cpp}`) refresh tty, xrdb, i3, bspwm, kitty, polybar, sway, and GTK. Skipped when `-e` is set; TTY reload skipped when `-t` is set.

Cross-cutting modules:

- `Setting` (`settings.h`) — global const paths (`HOME`, `XDG_CACHE_DIR`, `XDG_CONFIG_DIR`, `CACHE_DIR` = `$XDG_CACHE_HOME/wal`, `CONF_DIR` = `$XDG_CONFIG_HOME/wal`, `MODULE_DIR`, `OS`, `TERM`, `version`, `cacheVersion`). Many function defaults reference these — keep `settings.cpp` in sync when adding new paths.
- `Logging` (`logging.{h,cpp}`) — `info/error/exception`. Always log via this, never `qDebug`/`std::cout` directly in user-facing code.
- `AppException` (`appexception.{h,cpp}`) — `QException` subclass used for recoverable errors across the pipeline.
- `Util` (`util.{h,cpp}`) — file/JSON/process helpers (`readFile`, `saveFile`, `createDir`, `joinPath`, `which`, `run`, `pOpen`, `checkOutput`, `disown`, `runImageMagick`) and shared color helpers (`genericAdjust`, `saturateColors`, `colorsToMap`). This module is a kitchen-sink facade; prefer adding to it rather than creating new helper modules.
- `types.h` — `rgb_t`, `hls_t`, `yiq_t`, `ColorTuple = tuple<int, array<int,3>>`, `GroupedColors = array<array<array<vector<ColorTuple>,3>,3>,3>`, `RGBMaxInfo`.

## Conventions

- Includes in `src/*.cpp` use relative paths like `"../include/foo.h"` (see `main.cpp`). Follow the same style for new files.
- New source files must be added explicitly to the `qt_add_executable(walpp ...)` list in `CMakeLists.txt` — there is no glob.
- The `pywal/` directory is the reference Python implementation and is gitignored; consult it when porting behavior but do not modify it or add it to the build.
- `build/`, `cmake-build-debug/`, `.idea/`, `.vscode/` are all build/IDE artifacts — ignore.
