#pragma once
#include <QInputDialog>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <string>

class ModalBuilder {
    public:
        ModalBuilder();
        QString showInputModal(QWidget* parent, const QString& modal_label, const QString& input_label);
        void showErrorModal(const QString& message);
};