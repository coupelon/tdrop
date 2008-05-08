/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "systray.h"

Systray::Systray() {
    createTrayIcon();
    tdp.setEcho(false);
    (new daemonThread(&tdp))->start();
}

void Systray::createTrayIcon() {
    trayIconMenu = new QMenu();
    connect(trayIconMenu->addAction("&Preferences"), SIGNAL(triggered()), this, SLOT(showPreferences()));
    connect(trayIconMenu->addAction("&Quit"), SIGNAL(triggered()), qApp, SLOT(quit()));
		
    setContextMenu(trayIconMenu);
    setIcon(QIcon(":/images/teardrop.svg"));
}

void Systray::showPreferences() {
  prefDialog *pd = new prefDialog(&tdp);
  pd->exec();
  delete pd;
}


