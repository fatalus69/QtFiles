#include <QHeaderView>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDebug>

#include "file_explorer.h"
#include "file_operations.h"
#include "utils.h"
#include <iostream>
#include <string>

FileExplorer::FileExplorer(): QWidget()
{
  setWindowTitle("QtFiles");
  setFixedSize(800, 400); //Maybe dont do that.
  initUI();

  // Open Explorer in Users home directory
  current_path = getHomeDirectory();
  loadFiles(QString::fromStdString(current_path));
}

void FileExplorer::initUI()
{
  main_layout = new QVBoxLayout(this);
  menu_bar = new QMenuBar(this);
  QMenu *settings_menu = menu_bar->addMenu("File");

  QAction *open_settings_action = new QAction("Settings", this);
  connect(open_settings_action, &QAction::triggered, this, &FileExplorer::openSettings);
  settings_menu->addAction(open_settings_action);

  main_layout->setMenuBar(menu_bar);

  icon_cache["folder_filled"] = QPixmap("gui/assets/folder_filled.png");
  icon_cache["folder_empty"] = QPixmap("gui/assets/folder_empty.png");
  icon_cache["file"] = QPixmap("gui/assets/file.png");

  directory_display = new QLineEdit(this);
  connect(directory_display, &QLineEdit::returnPressed, this, &FileExplorer::onDirectoryEntered);

  search_bar = new QLineEdit(this);
  search_bar->setFixedWidth(200);
  search_bar->setPlaceholderText("Search for file");
  connect(search_bar, &QLineEdit::returnPressed, this, &FileExplorer::onSearchEntered);

  top_bar = new QHBoxLayout();
  top_bar->addWidget(directory_display);
  top_bar->addWidget(search_bar);
  main_layout->addLayout(top_bar);

  // Tree widget
  tree_widget = new QTreeWidget(this);
  tree_widget->setRootIsDecorated(false);
  tree_widget->setColumnCount(2);
  tree_widget->setHeaderLabels({"Name", "Size"});

  QHeaderView *header = tree_widget->header();
  header->setSectionResizeMode(0, QHeaderView::Stretch);
  header->setSectionResizeMode(1, QHeaderView::ResizeToContents);

  connect(tree_widget, &QTreeWidget::itemDoubleClicked,
          this, &FileExplorer::onItemDoubleClicked);

  main_layout->addWidget(tree_widget);
}

void FileExplorer::loadFiles(const QString &path)
{
  current_path = path.toStdString();
  directory_display->setText(path);
  
  /**
   * To see hidden files you'll have to change this value to false
   * until the Settings have been implemented.
   */
  bool hide_hidden_files = true;
  auto entries = listFiles(current_path, hide_hidden_files);

  createList(entries);
}


void FileExplorer::createList(auto entries, ListMode mode) {
  tree_widget->clear();

  for (const auto &file : entries) {
    QString icon_key = file.is_directory == true ? "folder_filled" : "file";

    QWidget *file_widget = new QWidget();
    QHBoxLayout *file_layout = new QHBoxLayout(file_widget);
    file_layout->setContentsMargins(4, 2, 4, 2);
    file_layout->setSpacing(6);

    QLabel *icon_label = new QLabel();
    icon_label->setPixmap(icon_cache[icon_key]);
    icon_label->setFixedSize(16, 16);
    icon_label->setScaledContents(true);

    std::string filename = (mode == ListMode::Search) ? file.path : file.name;

    QLabel *filename_label = new QLabel(QString::fromStdString(filename));
    file_layout->addWidget(icon_label);
    file_layout->addWidget(filename_label);
    file_layout->addStretch();

    QLabel *filesize_label = new QLabel(QString::fromStdString(file.size));

    QTreeWidgetItem *item = new QTreeWidgetItem({"", ""});
    item->setData(0, Qt::UserRole, QString::fromStdString(filename));
    item->setData(0, Qt::UserRole + 1, file.is_directory);

    tree_widget->addTopLevelItem(item);
    tree_widget->setItemWidget(item, 0, file_widget);
    tree_widget->setItemWidget(item, 1, filesize_label);
  }
}

void FileExplorer::onDirectoryEntered()
{
  std::string dir_path = directory_display->text().toStdString();
  loadFiles(QString::fromStdString(dir_path));
}

void FileExplorer::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
  QString filepath = item->data(0, Qt::UserRole).toString();
  bool is_directory = item->data(0, Qt::UserRole + 1).toBool();

  if (is_directory == true) {
    std::string filepath_string = filepath.toStdString();
    if (filepath_string.find(current_path) == std::string::npos) {
      filepath_string = current_path + "/" + filepath_string;
    }

    loadFiles(QString::fromStdString(filepath_string));
  }
}


void FileExplorer::onSearchEntered()
{
  std::string query = search_bar->text().toStdString();
  auto search_result = searchDirectory(current_path, query);

  createList(search_result, ListMode::Search);
}


/**
 * We'll ignore settings for now.
 */
void FileExplorer::openSettings()
{
  qDebug() << "Open settings (Not implemented yet)";
  //open settings
}
