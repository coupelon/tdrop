#include <QApplication>
#include <QPushButton>

#include "view/myWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  myWindow * mw = new myWindow();
  mw->show();
  return app.exec();
}
