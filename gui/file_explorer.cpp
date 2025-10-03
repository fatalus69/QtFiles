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
  setFixedSize(800, 400);
  initUI();

  current_path = get_home_directory();
  loadFiles(QString::fromStdString(current_path));
}

void FileExplorer::initUI()
{
  mainLayout = new QVBoxLayout(this);
  menuBar = new QMenuBar(this);
  QMenu *settingsMenu = menuBar->addMenu("File");

  QAction *openSettingsAction = new QAction("Settings", this);
  connect(openSettingsAction, &QAction::triggered, this, &FileExplorer::openSettings);
  settingsMenu->addAction(openSettingsAction);

  mainLayout->setMenuBar(menuBar);

  icon_cache["folder_filled"] = QPixmap("gui/assets/folder_filled.png");
  icon_cache["folder_empty"] = QPixmap("gui/assets/folder_empty.png");
  icon_cache["file"] = QPixmap("gui/assets/file.png");

  directoryDisplay = new QLineEdit(this);
  connect(directoryDisplay, &QLineEdit::returnPressed, this, &FileExplorer::onDirectoryEntered);

  search_bar = new QLineEdit(this);
  search_bar->setFixedWidth(200);
  search_bar->setPlaceholderText("Search");
  connect(search_bar, &QLineEdit::returnPressed, this, &FileExplorer::onSearchEntered);

  topBar = new QHBoxLayout();
  topBar->addWidget(directoryDisplay);
  topBar->addWidget(search_bar);
  mainLayout->addLayout(topBar);

  // Tree widget
  treeWidget = new QTreeWidget(this);
  treeWidget->setRootIsDecorated(false);
  treeWidget->setColumnCount(2);
  treeWidget->setHeaderLabels({"Name", "Size"});

  QHeaderView *header = treeWidget->header();
  header->setSectionResizeMode(0, QHeaderView::Stretch);
  header->setSectionResizeMode(1, QHeaderView::ResizeToContents);

  connect(treeWidget, &QTreeWidget::itemDoubleClicked,
          this, &FileExplorer::onItemDoubleClicked);

  mainLayout->addWidget(treeWidget);
}

void FileExplorer::loadFiles(const QString &path)
{
  current_path = path.toStdString();
  directoryDisplay->setText(path);
  treeWidget->clear();
  
  auto entries = list_files(current_path, false);

  for (const auto &entry : entries)
  {
    QString iconKey = entry.is_directory == true ? "folder_filled" : "file";

    QWidget *fileWidget = new QWidget();
    QHBoxLayout *fileLayout = new QHBoxLayout(fileWidget);
    fileLayout->setContentsMargins(4, 2, 4, 2);
    fileLayout->setSpacing(6);

    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(icon_cache[iconKey]);
    iconLabel->setFixedSize(16, 16);
    iconLabel->setScaledContents(true);

    QLabel *filenameLabel = new QLabel(QString::fromStdString(entry.filename));
    fileLayout->addWidget(iconLabel);
    fileLayout->addWidget(filenameLabel);
    fileLayout->addStretch();

    QLabel *filesizeLabel = new QLabel(QString::fromStdString(entry.filesize));

    QTreeWidgetItem *item = new QTreeWidgetItem({"", ""});
    item->setData(0, Qt::UserRole, QString::fromStdString(entry.filename));
    item->setData(0, Qt::UserRole + 1, entry.is_directory);

    treeWidget->addTopLevelItem(item);
    treeWidget->setItemWidget(item, 0, fileWidget);
    treeWidget->setItemWidget(item, 1, filesizeLabel);
  }
}

void FileExplorer::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
  QString filepath = item->data(0, Qt::UserRole).toString();
  bool is_directory = item->data(0, Qt::UserRole + 1).toBool();

  if (is_directory == true) {
    std::string filepath_string = current_path + "/" + filepath.toStdString();
    loadFiles(QString::fromStdString(filepath_string));
  }
}

void FileExplorer::onDirectoryEntered()
{
  std::string directory_text = directoryDisplay->text().toStdString();
  loadFiles(QString::fromStdString(directory_text));
}

void FileExplorer::onSearchEntered()
{
  QString searchText = search_bar->text();
  qDebug() << "Search for:" << searchText;
  //search
}

void FileExplorer::openSettings()
{
  qDebug() << "Open settings";
  //open settings
  // loadFiles(current_path);
}
