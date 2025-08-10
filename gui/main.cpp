#include <QApplication>
#include "file_explorer.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  FileExplorer window;

  window.show();
  return app.exec();
}
