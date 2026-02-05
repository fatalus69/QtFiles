#ifndef CLICKABLE_LINE_EDIT_H
#define CLICKABLE_LINE_EDIT_H

#include <QLineEdit>
#include <QMouseEvent>

class ClickableLineEdit : public QLineEdit {
    Q_OBJECT

public:
    using QLineEdit::QLineEdit;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        QLineEdit::mousePressEvent(event);
        selectAll();
        emit clicked();
    }
};

#endif