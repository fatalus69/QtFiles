#pragma once

#include <QInputDialog>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>

#include <string>

#include "../../core/file_operations.h"
#include "../../utils/types.h"
#include "../../utils/utils.h"

class ModalBuilder {
    public:
        ModalBuilder();
        QString showInputModal(QWidget* parent, const QString& modal_label, const QString& input_label);
        void showErrorModal(const QString& message);
        void showPropertiesModal(const FileEntry& file_entry);
};