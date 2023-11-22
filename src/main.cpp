#include "RpBevel.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  RpBevel w;
  w.show();
  return a.exec();
}
