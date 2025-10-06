#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QTreeWidget>
#include <QPixmap>
#include <QTimer>
#include <QSize>
#include <QKeyEvent>
#include <QDebug>

#include <string>

#include "file_operations.h"
#include "utils.h"

class FileExplorer : public QWidget 
{
public:
  FileExplorer();

private slots:
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onDirectoryEntered();
    void onSearchEntered();
    void openSettings();
    void keyPressEvent(QKeyEvent *event);
    void handleRename(QTreeWidgetItem *item);

    // Extensible for Favourites, etc.
    enum class ListMode {
      Normal,
      Search
    };

private:
    void initUI();
    void loadFiles(const QString &path);
    void createList(auto entries, ListMode mode = ListMode::Normal);

    QVBoxLayout *main_layout;
    QHBoxLayout *top_bar;

    QMenuBar *menu_bar;
    QLineEdit *directory_display;
    QLineEdit *search_bar;
    QTreeWidget *tree_widget;

    QMap<QString, QPixmap> icon_cache;

    std::string current_path;
};
