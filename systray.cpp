#include "systray.h"

Systray::Systray() {
    createTrayIcon();
}

void Systray::createTrayIcon() {
    trayIconMenu = new QMenu();
    connect(trayIconMenu->addAction("&Quit"), SIGNAL(triggered()), qApp, SLOT(quit()));

    setContextMenu(trayIconMenu);
    setIcon(QIcon(":/images/teardrop.svg"));
}



