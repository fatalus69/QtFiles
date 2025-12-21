#include "file_model.h"

FileModel::FileModel(QObject *parent) : QAbstractTableModel(parent) {
  icon_cache["folder_filled"] = QIcon(":/icons/folder_filled.png");
  icon_cache["folder_empty"] = QIcon(":/icons/folder_empty.png");
  icon_cache["file"] = QIcon(":/icons/file.png");
}

int FileModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }
  return static_cast<int>(files.size());
}

int FileModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }

  return 2;
}

void FileModel::setFiles(std::vector<FileEntry> new_files) {
  beginResetModel();
  files = std::move(new_files);
  endResetModel();
}

QVariant FileModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal){
    return {};
  }
  switch (section) {
    case 0:
      return "Name";
    case 1:
      return "Size";
  }

  return {};
}

const FileEntry &FileModel::fileAt(int row) const
{
  return files[row];
}

QVariant FileModel::data(const QModelIndex &model_index, int role) const
{
  if (role != Qt::DisplayRole) {
    return {};
  }

  const auto &files_from_index = files[model_index.row()];

  // TODO: expand with settings
  if (role == Qt::DisplayRole) {
    if (model_index.column() == 0) {
      return QString::fromStdString(files_from_index.name);
    }
  
    if (model_index.column() == 1) {
      return formatByte(files_from_index.size);
    }
  }

  if (role == Qt::DecorationRole && model_index.column() == 0) {
      return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
    if (files_from_index.type == FileType::Directory) {
      return icon_cache["folder_filled"];
    } else {
      return icon_cache["file"];
    }
  }

  return {};
}

Qt::ItemFlags FileModel::flags(const QModelIndex &model_index) const {
  if (!model_index.isValid()) {
    return Qt::NoItemFlags;
  }

  Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  if (model_index.column() == 0) {
    flags |= Qt::ItemIsEditable;
  }

  return flags;
}

bool FileModel::setData(const QModelIndex &model_index, const QVariant &value, int role) {
  if (!model_index.isValid()) {
    return false;
  }

  if (role != Qt::EditRole || model_index.column() != 0) {
    return false;
  }

  auto &file = files[model_index.row()];
  QString new_name = value.toString().trimmed();

  if (new_name.isEmpty() || new_name == QString::fromStdString(file.name)) {
    return false;
  }

  std::filesystem::path old_path = file.path;
  std::filesystem::path new_path = old_path.parent_path() / new_name.toStdString();

  if (!renameFile(old_path.string(), new_path.string())) {
    return false;
  }

  file.name = new_name.toStdString();
  file.path = new_path.string();

  emit dataChanged(model_index, model_index);
  return true;
}
