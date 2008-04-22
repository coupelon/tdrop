#include "systray.h"

Systray::Systray() {
    createTrayIcon();
    tdp.setEcho(false);
    (new daemonThread(&tdp))->start();
}

void Systray::createTrayIcon() {
    trayIconMenu = new QMenu();
    connect(trayIconMenu->addAction("&Quit"), SIGNAL(triggered()), qApp, SLOT(quit()));

    setContextMenu(trayIconMenu);
    setIcon(QIcon(":/images/teardrop.svg"));
}



