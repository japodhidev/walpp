#ifndef RELOAD_H
#define RELOAD_H

#include <qcontainerfwd.h>

class Reload
{
public:
    Reload();
    static void env(bool ttyReload = true);
    static void gtk();
private:
    static void tty(bool ttyReload);
    static void xrdb();
    static void i3();
    static void bspwm();
    static void kitty();
    static void polybar();
    static void sway();

};

#endif // RELOAD_H
