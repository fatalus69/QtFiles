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