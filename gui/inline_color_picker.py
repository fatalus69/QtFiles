from PyQt5.QtWidgets import QWidget, QHBoxLayout, QLabel, QPushButton, QColorDialog
from PyQt5.QtGui import QColor

class InlineColorPicker(QWidget):
    def __init__(self, label_text="Select Color", default_color="#ffffff", parent=None):
        super().__init__(parent)
        self.color = QColor(default_color)

        layout = QHBoxLayout()
        layout.addWidget(QLabel(label_text))

        self.button = QPushButton()
        self.update_button_color()
        self.button.clicked.connect(self.choose_color)

        layout.addWidget(self.button)
        layout.addStretch()
        self.setLayout(layout)

    def update_button_color(self):
        self.button.setStyleSheet(
            f"background-color: {self.color.name()}; border: 1px solid #000;"
        )
        self.button.setFixedSize(40, 20)

    def choose_color(self):
        color = QColorDialog.getColor(self.color, self, "Select Color")
        if color.isValid():
            self.color = color
            self.update_button_color()

    def get_color(self):
        return self.color.name()
