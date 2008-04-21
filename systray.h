#ifndef SYSTRAY
#define SYSTRAY

#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>

class Systray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    Systray();

private:
    void createTrayIcon();

    QMenu *trayIconMenu;
};

#endif
