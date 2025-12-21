#pragma once

#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>
#include <qabstractitemmodel.h>
#include <qvariant.h>
#include <QMap>
#include <QIcon>
#include <QApplication>
#include <QStyle>

#include <vector>
#include <filesystem>

#include "../../utils/types.h"
#include "../../utils/utils.h"
#include "../../core/file_operations.h"

class FileModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit FileModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  bool setData(const QModelIndex& model_index, const QVariant& value, int role = Qt::EditRole) override;
  QVariant data(const QModelIndex& model_index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex& model_index) const override;

  void setFiles(std::vector<FileEntry> files);
  const FileEntry& fileAt(int row) const;

private:
  QMap<QString, QIcon> icon_cache;
  std::vector<FileEntry> files;
};
