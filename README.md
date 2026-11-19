# walpp

A rewrite of __MOST__ of [pywal's](https://github.com/dylanaraps/pywal) functionality using [QT](https://www.qt.io/).
***Has only been tested on Linux at the moment.***.

### Motivation

Due to Arch Linux's fast-moving rolling updates nature, I found it a bit tiring to re-install Pywal with every new Python version.
Managing `venv`s is quite a bit daunting and a little too much effort for me.

The versatility provided by a single executable, IMO, provides a leg up from managing different Python modules. This is (of course) undermined
by the fact that support for additional backends has to be built from the ground up.

### Vision

- Implement some if not all of [pywal's](https://github.com/dylanaraps/pywal) supported backends available to `walpp`. I like `haishoku`.
- Test, package and release executables, if needed.
- Support `Windows` & `MacOS`.

## Building

### Prerequisites
- [CMake](https://cmake.org/) ≥ 3.x
- [Ninja](https://ninja-build.org/) build system
- A C++ compiler (GCC, Clang, etc.)

**1. Configure the project**

```bash
cmake -DCMAKE_BUILD_TYPE=RELEASE -G Ninja -S . -B ./build
```

**2. Build the target**

```bash
cmake --build ./build --target walpp
```

> To speed up compilation, increase the parallelism flag `-j` to match your CPU core count, e.g. `-j 4`.

---

**Tip:** If Ninja is not on your `PATH`, point CMake to it explicitly:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/path/to/ninja -G Ninja -S . -B ./build
```


## Usage

The arguments are similar to [pywal's](https://github.com/dylanaraps/pywal)

```sh
wal - Generate colorschemes on the fly

Options:
  -a, --alpha <alpha>                    Set terminal background transparency.
                                         *Only works in URxvt*
  -b, --background <background>          Custom background color to use.
  --backend, --backend <backend>         Which color backend to use. Use 'walpp
                                         --backend' to list backends.
  --theme, --theme <theme>               Which colorscheme file to use. Use
                                         'walpp --theme' to list builtin and
                                         user themes.
  --iterative                            When walpp is given a directory as
                                         input and this flag is used: Go through
                                         the images in order instead of
                                         shuffled.
  --recursive                            When walpp is given a directory as
                                         input and this flag is used: Search for
                                         images recursively in subdirectories
                                         instead of the root only.
  --saturate, --saturate <saturate>      Set the color saturation.
  --preview                              Print the current color palette.
  --vte                                  Fix text-artifacts printed in VTE
                                         terminals.
  -c                                     Delete all cached colorschemes.
  -i, --directory <directory>            Which image or directory to use.
  -l                                     Generate a light colorscheme.
  -n                                     Skip setting the wallpaper.
  -o, --externalScript <externalScript>  External script to run after 'wal'.
  -p, --saveTheme <saveTheme>            Permanently save theme to
                                         $XDG_CONFIG_HOME/wal/colorschemes with
                                         the specified name.
  -q                                     Quiet mode, don't print anything.
  -s                                     Skip changing colors in terminals.
  -t                                     Skip changing colors in tty.
  -v                                     Print walpp version.
  -w                                     Use last used wallpaper for color
                                         generation.
  -e                                     Skip reloading
                                         gtk/xrdb/i3/sway/polybar
```


