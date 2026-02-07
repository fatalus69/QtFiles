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
#include <QInputDialog>
#include <filesystem>

#include <string>
#include <sstream>
#include <vector>

#include "../utils/types.h"
#include "../utils/utils.h"
#include "../core/file_operations.h"
#include "builders/modal_builder.h"
#include "models/file_model.h"
#include "components/inputs/clickable_line_edit.h"

class FileExplorer : public QWidget 
{
Q_OBJECT
public:
  FileExplorer(std::string initial_path);

private slots:
    void onItemActivated(const QModelIndex& index);
    void onDirectoryEntered();
    void onSearchEntered();
    void openSettings();
    void keyPressEvent(QKeyEvent *event);
    void handleDelete(const QModelIndex& index);
    void handleEntityCreate(FileType type);
    void showContextMenu(const QPoint &pos);

private:
    void initUI();
    void loadFiles(const QString &path);

    QVBoxLayout *main_layout;
    QHBoxLayout *top_bar;

    QMenuBar *menu_bar;
    ClickableLineEdit *directory_display;
    QLineEdit *search_bar;
    QTreeView *tree_view;
    FileModel* file_model;

    QMap<QString, QPixmap> icon_cache;

    std::string current_path;
    ModalBuilder modal_builder;
};
