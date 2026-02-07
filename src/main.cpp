#include <QApplication>
#include <filesystem>

#include "gui/file_explorer.h"

int main(int argc, char *argv[]) {
  std::string path;

  // TODO: build a proper argument parse for stuff like --debug, ...
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (std::filesystem::exists(argv[i]) && std::filesystem::is_directory(argv[i])) {
        path = argv[i];
        break;
      }
    }
  }

  QApplication app(argc, argv);
  FileExplorer window(path);

  window.show();
  return app.exec();
}
