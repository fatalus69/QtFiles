#include "file_explorer.h"

#include <iostream>


FileExplorer::FileExplorer(): QWidget()
{
  setWindowTitle("QtFiles");
  setFixedSize(800, 400); //Maybe dont do that.
  initUI();

  this->modal_builder = ModalBuilder();

  // Open Explorer in Users home directory
  current_path = getHomeDirectory();
  loadFiles(QString::fromStdString(current_path));
}

void FileExplorer::initUI()
{
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QWidget::customContextMenuRequested, this, &FileExplorer::showContextMenu);

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
  
  auto entries = listFiles(current_path);

  createList(entries);
}


void FileExplorer::createList(auto entries, ListMode mode) {
  tree_widget->clear();

  for (const auto &file : entries) {
    QString icon_key = file.type == FileType::Directory ? "folder_filled" : "file";

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
    filename_label->setObjectName("filename_label");
    file_layout->addWidget(icon_label);
    file_layout->addWidget(filename_label);
    file_layout->addStretch();

    QLabel *filesize_label = new QLabel(formatByte(file.size));

    QTreeWidgetItem *item = new QTreeWidgetItem({"", ""});
    item->setData(0, Qt::UserRole, QString::fromStdString(filename));
    item->setData(0, Qt::UserRole + 1, file.type == FileType::Directory);

    tree_widget->addTopLevelItem(item);
    tree_widget->setItemWidget(item, 0, file_widget);
    tree_widget->setItemWidget(item, 1, filesize_label);
  }
}

void FileExplorer::keyPressEvent(QKeyEvent *event) {
  QTreeWidgetItem *item = tree_widget->currentItem();

  switch(event->key()) 
  {
    case Qt::Key_F2: {
      if (item) {
        handleRename(item);
      }  
    }
    case Qt::Key_Delete: {
      if (item) {
        handleDelete(item);
      }
    }
    default:
      QWidget::keyPressEvent(event);
  }
}

void FileExplorer::showContextMenu(const QPoint &pos) 
{
  QAction *create_file_action = new QAction("File", this);
  QAction *create_directory_action = new QAction("Directory", this);

  // Using lambda, since we need the argument what we want to create.
  connect(create_file_action, &QAction::triggered, this,
    [this]() { handleEntityCreate(FileType::File); });

  connect(create_directory_action, &QAction::triggered, this,
    [this]() { handleEntityCreate(FileType::Directory); });
  QMenu context_menu(tr("Context menu"), this);

  QMenu new_entity_menu("New", this);
  new_entity_menu.addAction(create_file_action);
  new_entity_menu.addAction(create_directory_action);

  context_menu.addMenu(&new_entity_menu);

  context_menu.exec(mapToGlobal(pos));
}

void FileExplorer::handleEntityCreate(FileType type) {
  QString label_text = QString("Create new ") + QString(type == FileType::File ? "File" : "Directory");
  std::string name = this->modal_builder.showInputModal(this, label_text, "Name").toStdString();

  if (name != "") {
    std::filesystem::path path(current_path + PATH_SEPARATOR + name);
    bool created_entity = createFile(path, type);

    if (!created_entity) {
      QString error_message = QString("Failed to create ") + QString(type == FileType::File ? "file." : "directory.");
      this->modal_builder.showErrorModal(error_message);

      return;
    }

    loadFiles(QString::fromStdString(current_path));
  }
}

void FileExplorer::handleRename(QTreeWidgetItem *item) {
  QWidget *file_widget = tree_widget->itemWidget(item, 0);
  if (!file_widget) return;

  QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(file_widget->layout());
  if (!layout) return;

  QLabel *filename_label = file_widget->findChild<QLabel *>("filename_label");
  if (!filename_label) return;

  QString filename = filename_label->text();

  QLineEdit *edit = new QLineEdit(filename, file_widget);
  edit->selectAll();
  layout->replaceWidget(filename_label, edit);
  filename_label->hide();

  edit->setFocus();

  connect(edit, &QLineEdit::editingFinished, this, [this, filename, edit, filename_label, item]() {
    QString new_name = edit->text().trimmed();

    if (!new_name.isEmpty() && new_name != filename_label->text()) {
      std::string filepath = current_path + PATH_SEPARATOR + filename.toStdString();

      std::string new_path = current_path + PATH_SEPARATOR + new_name.toStdString();

      renameFile(filepath, new_path);
    }

    // Reload current directory after changing filenames
    loadFiles(QString::fromStdString(current_path));
  });
}

void FileExplorer::handleDelete(QTreeWidgetItem *item) {
  QWidget *file_widget = tree_widget->itemWidget(item, 0);
  if (!file_widget) return;

  QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(file_widget->layout());
  if (!layout) return;

  QLabel *filename_label = file_widget->findChild<QLabel *>("filename_label");
  if (!filename_label) return;

  QString filename = filename_label->text();
  std::string filepath = current_path + PATH_SEPARATOR + filename.toStdString();
  bool deleted = deleteFile(filepath);

  if (!deleted) {
    this->modal_builder.showErrorModal("Failed to delete the file or directory.");
    return;
  }

  loadFiles(QString::fromStdString(current_path));
}

void FileExplorer::onDirectoryEntered()
{
  std::string dir_path = directory_display->text().toStdString();
  std::filesystem::path path(dir_path);

  if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
    this->modal_builder.showErrorModal("The specified path does not exist.");
    directory_display->setText(QString::fromStdString(current_path));
    return;
  }

  loadFiles(QString::fromStdString(dir_path));
}

void FileExplorer::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
  QString filepath = item->data(0, Qt::UserRole).toString();
  bool is_directory = item->data(0, Qt::UserRole + 1).toBool();

  if (is_directory) {
    std::string filepath_string = filepath.toStdString();
    if (filepath_string.find(current_path) == std::string::npos) {
      filepath_string = current_path + PATH_SEPARATOR + filepath_string;
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
