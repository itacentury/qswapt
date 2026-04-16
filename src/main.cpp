#include <QApplication>

#include "mainwindow.h"
#include "station_reader.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  readStations();
  return QApplication::exec();
}
