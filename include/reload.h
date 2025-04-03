#ifndef RELOAD_H
#define RELOAD_H

#include <qcontainerfwd.h>

class Reload
{
public:
    Reload();
    static void env(QStringList xrdbFile, bool ttyReload = true);
    static void gtk();
private:
    static void tty(bool ttyReload);
    static void xrdb(QStringList xrdbFiles);
    static void i3();
    static void bspwm();
    static void kitty();
    static void polybar();
    static void sway();

};

#endif // RELOAD_H
