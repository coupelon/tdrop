#ifndef SYSTRAY
#define SYSTRAY

#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>

#include "daemonThread.h"

class Systray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    Systray();

private:
    void createTrayIcon();
    
    QMenu *trayIconMenu;
    tdParam tdp;
};

#endif
