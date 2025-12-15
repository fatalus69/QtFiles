#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include "file_explorer.h"

int main(int argc, char *argv[]) {
  
  QApplication app(argc, argv);
  FileExplorer window;
  
  QQmlApplicationEngine engine;
  engine.load(QUrl("gui/qml/main.qml"));
  
  window.show();
  return app.exec();
}
