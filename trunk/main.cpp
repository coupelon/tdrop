#include <QtGui>

#include "systray.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Teardrop"),
                              QObject::tr("Your system doesn't have a system tray."));
        return 1;
    }

    Systray systray;
    systray.show();
    
    return app.exec();
}
