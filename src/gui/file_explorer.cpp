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

  directory_display = new ClickableLineEdit(this);
  connect(directory_display, &ClickableLineEdit::clicked, this, [this]() {
    directory_display->selectAll();
  });
  connect(directory_display, &QLineEdit::returnPressed, this, &FileExplorer::onDirectoryEntered);

  search_bar = new QLineEdit(this);
  search_bar->setFixedWidth(200);
  search_bar->setPlaceholderText("Search for file");
  search_bar->setSelection(0, 0);
  connect(search_bar, &QLineEdit::returnPressed, this, &FileExplorer::onSearchEntered);

  top_bar = new QHBoxLayout();
  top_bar->addWidget(directory_display);
  top_bar->addWidget(search_bar);
  main_layout->addLayout(top_bar);

  // Tree widget
  tree_view = new QTreeView(this);
  tree_view->setRootIsDecorated(false);
  tree_view->setAlternatingRowColors(true);
  tree_view->setUniformRowHeights(true);

  file_model = new FileModel(this);
  tree_view->setModel(file_model);

  tree_view->setRootIsDecorated(false);
  tree_view->setAlternatingRowColors(true);
  tree_view->setSortingEnabled(true);
  tree_view->setSelectionMode(QAbstractItemView::SingleSelection);
  tree_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  tree_view->setIconSize(QSize(16,16));

  tree_view->setEditTriggers(
    QAbstractItemView::EditKeyPressed |
    QAbstractItemView::SelectedClicked
  );

  QHeaderView *header = tree_view->header();
  header->setSectionResizeMode(0, QHeaderView::Stretch);
  header->setSectionResizeMode(1, QHeaderView::ResizeToContents);

  connect(tree_view, &QTreeView::doubleClicked,
          this, &FileExplorer::onItemActivated);

  main_layout->addWidget(tree_view);
}

void FileExplorer::loadFiles(const QString &path)
{
  directory_display->setText(path);
  
  std::vector<FileEntry> entries = listFiles(path.toStdString());
  file_model->setFiles(std::move(entries));

  current_path = path.toStdString();
}

void FileExplorer::keyPressEvent(QKeyEvent *event) {
  QModelIndex index = tree_view->currentIndex();

  if (!index.isValid())
      return;

  switch(event->key()) 
  {
    case Qt::Key_F2: {
      tree_view->edit(index);
      break;
    }
    case Qt::Key_Delete: {
      handleDelete(index);
      break;
    }
    default:
      QWidget::keyPressEvent(event);
  }
}

void FileExplorer::showContextMenu(const QPoint &pos) 
{
  QModelIndex index = tree_view->currentIndex();

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
 
  //TODO: Add rename and delete actions for index
  if (index.isValid()) {
    QAction *rename_action = new QAction("Rename", this);
    QAction *delete_action = new QAction("Delete", this);

    connect(rename_action, &QAction::triggered, this,
      [this, index]() { tree_view->edit(index); });

    connect(delete_action, &QAction::triggered, this,
      [this, index]() { handleDelete(index); });

    context_menu.addAction(rename_action);
    context_menu.addAction(delete_action);
  }

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

void FileExplorer::handleDelete(const QModelIndex& index) {
  if (!index.isValid())
    return;

  const FileEntry& file = file_model->fileAt(index.row());

  bool deleted = deleteFile(file.path);
  if (!deleted) {
      modal_builder.showErrorModal("Failed to delete the file or directory.");
      return;
  }

  loadFiles(QString::fromStdString(current_path));
}

void FileExplorer::onDirectoryEntered()
{
  std::string dir_path = directory_display->text().toStdString();
  std::filesystem::path path(dir_path);

  try {
    loadFiles(QString::fromStdString(dir_path));
  } catch(const std::filesystem::filesystem_error& e) {
    qDebug() << "Error opening directory: " << e.what();

    std::istringstream error_stream(e.what());
    std::vector<std::string> error_parts;
    std::string error_part;

    while (std::getline(error_stream, error_part, ':')) {
      error_parts.push_back(error_part);
    }

    this->modal_builder.showErrorModal("Failed to open directory. " + QString::fromStdString(error_parts.back()));
    directory_display->setText(QString::fromStdString(current_path));
    return;
  }
}

void FileExplorer::onItemActivated(const QModelIndex& index)
{
  if (!index.isValid())
        return;

    const FileEntry& file = file_model->fileAt(index.row());

    if (file.type == FileType::Directory) {
        loadFiles(QString::fromStdString(file.path));
    }
}

void FileExplorer::onSearchEntered()
{
  std::string query = search_bar->text().toStdString();
  std::vector<FileEntry> search_result = searchDirectory(current_path, query);

  // TODO: fix display showing everything, like it would with loadFiles
  file_model->setFiles(std::move(search_result));
}

/**
 * We'll ignore settings for now.
 */
void FileExplorer::openSettings()
{
  qDebug() << "Open settings (Not implemented yet)";
  //open settings
}
