#ifndef RELOAD_H
#define RELOAD_H

#include <qcontainerfwd.h>

class Reload
{
public:
    Reload();
    void env(QStringList xrdbFile, bool ttyReload = true);
    void gtk();
private:
    void tty(bool ttyReload);
    void xrdb(QStringList xrdbFiles);
    void i3();
    void bspwm();
    void kitty();
    void polybar();
    void sway();

};

#endif // RELOAD_H
