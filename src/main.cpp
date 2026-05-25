#include <QObject>
#include <QQmlApplicationEngine>
#include <QtGui/QGuiApplication>

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  engine.loadFromModule("App.QSwapt", "Main");

  return QGuiApplication::exec();
}
