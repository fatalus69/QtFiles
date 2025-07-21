from PyQt5.QtWidgets import QDialog, QVBoxLayout, QHBoxLayout, QLabel, QColorDialog, QPushButton, QWidget, QLineEdit
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QIntValidator
from inline_colour_picker import InlineColorPicker
import fileops


class Settings(QDialog):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Settings - QtFiles")
        
        dialog_layout = QVBoxLayout()
        
        # Font size
        font_size_layout = QVBoxLayout()
        font_size_layout.addWidget(QLabel("Font Size"))
        
        font_size_edit = QLineEdit()
        font_size_edit.setValidator(QIntValidator(0, 100, self))
        font_size_layout.addWidget(font_size_edit)

        dialog_layout.addLayout(font_size_layout)        
        
        # Font Colour
        self.font_colour_picker = InlineColorPicker("Font Colour", "#cccccc")
        dialog_layout.addWidget(self.font_colour_picker)
        
        # Bg-colour
        self.bg_colour_picker = InlineColorPicker("Background Colour", "#cccccc")
        dialog_layout.addWidget(self.bg_colour_picker)

        # Selected Item bg-colour
        self.selected_bg_colour_picker = InlineColorPicker("Selected Item Background Colour", "#cccccc")
        dialog_layout.addWidget(self.selected_bg_colour_picker)
        
        # selected Item height
        # Icon-size (only allow 0.8 * selected item height)

        button_layout = QHBoxLayout()

        cancel_button = QPushButton("Cancel")
        cancel_button.clicked.connect(self.reject)

        save_button = QPushButton("Save")
        save_button.clicked.connect(self.on_save_click)

        button_layout.addWidget(save_button)
        button_layout.addWidget(cancel_button)
        dialog_layout.addLayout(button_layout)

        self.setLayout(dialog_layout)
        self.exec_()
        
    def on_save_click(self):
        #fileops.saveFile(data)
        self.accept()