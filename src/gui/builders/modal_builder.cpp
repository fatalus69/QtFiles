#include "modal_builder.h"

ModalBuilder::ModalBuilder() {

}

QString ModalBuilder::showInputModal(QWidget* parent, const QString& modal_label, const QString& input_label) {
    QInputDialog *modal = new QInputDialog(parent);

    modal->setInputMode(QInputDialog::TextInput);

    modal->setWindowTitle(modal_label);
    modal->setLabelText(input_label);
    modal->setTextValue("");
    
    modal->setOkButtonText("OK");
    modal->setCancelButtonText("Cancel");
    modal->setModal(true);

    if (modal->exec() == QInputDialog::Accepted) {
        return modal->textValue();
    }

    return QString();
}

void ModalBuilder::showErrorModal(const QString& message) {
    QMessageBox msg_box;

    msg_box.setIcon(QMessageBox::Critical);
    msg_box.setText("Error");
    
    msg_box.setInformativeText(message);
    msg_box.setStandardButtons(QMessageBox::Ok);
    msg_box.exec();
}

void ModalBuilder::showPropertiesModal(const FileEntry& file_entry) {
    QMessageBox msg_box;
    FileProperties properties = getFileProperties(file_entry.path);

    msg_box.setIcon(QMessageBox::Information);
    msg_box.setWindowTitle("Properties");
    msg_box.setText(QString::fromStdString(file_entry.name));

    QString details = QString("Type: ") + (file_entry.type == FileType::Directory ? "Directory" : "File") + "\n";
    details += QString("Size: ") + formatByte(file_entry.size) + "\n";
    details += QString("Path: ") + QString::fromStdString(file_entry.path) + "\n";

    details += QString("Last Access Time: ") + formatTimestamp(properties.access_time) + "\n";
    details += QString("Modification Time: ") + formatTimestamp(properties.modification_time) + "\n";
    details += QString("Creation Time: ") + formatTimestamp(properties.creation_time) + "\n";

    msg_box.setInformativeText(details);
    msg_box.setStandardButtons(QMessageBox::Ok);

    msg_box.exec();
}