from PyQt5.QtWidgets import QDialog, QVBoxLayout, QHBoxLayout, QLabel, QColorDialog, QPushButton, QWidget, QLineEdit
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QIntValidator
from inline_color_picker import InlineColorPicker
import fileops


class Settings(QDialog):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Settings - QtFiles")
        self.settings = fileops.Settings()
        
        self.setting_font_size = self.settings.get_setting("font_size")
        self.setting_font_color = self.settings.get_setting("font_color")
        self.setting_bg_color = self.settings.get_setting("bg_color")
        self.setting_selected_bg_color = self.settings.get_setting("selected_bg_color") # highlight color of selcted item
        self.setting_item_height = self.settings.get_setting("item_height")
        self.setting_icon_size = self.settings.get_setting("icon_size")
        
        dialog_layout = QVBoxLayout()
        
        # Font size
        font_size_layout = QVBoxLayout()
        font_size_layout.addWidget(QLabel("Font Size"))
        
        self.font_size_edit = QLineEdit()
        self.font_size_edit.setValidator(QIntValidator(0, 40, self))
        self.font_size_edit.setText(self.setting_font_size)
        font_size_layout.addWidget(self.font_size_edit)

        dialog_layout.addLayout(font_size_layout)
        
        # selected Item height
        file_item_height_layout = QVBoxLayout()
        file_item_height_layout.addWidget(QLabel("Selected item height"))
        
        self.file_item_height_edit = QLineEdit()
        self.file_item_height_edit.setValidator(QIntValidator(0,100, self))
        self.file_item_height_edit.setText(self.setting_item_height)
        file_item_height_layout.addWidget(self.file_item_height_edit)

        dialog_layout.addLayout(file_item_height_layout)        
        
        # Font color
        self.font_color_picker = InlineColorPicker("Font color", self.setting_font_color)
        dialog_layout.addWidget(self.font_color_picker)
        
        # Bg-color
        self.bg_color_picker = InlineColorPicker("Background color", self.setting_bg_color)
        dialog_layout.addWidget(self.bg_color_picker)

        # Selected Item bg-color
        self.selected_bg_color_picker = InlineColorPicker("Selected Item Background color", self.setting_selected_bg_color)
        dialog_layout.addWidget(self.selected_bg_color_picker)
        
        # future Icon-size (only allow 0.8 * selected item height)

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
        setting_entries = [
            ("font_size", self.setting_font_size, self.font_size_edit.text()),
            ("font_color", self.setting_font_color, self.font_color_picker.get_color()),
            ("bg_color", self.setting_bg_color, self.bg_color_picker.get_color()),
            ("selected_bg_color", self.setting_selected_bg_color, self.selected_bg_color_picker.get_color()),
            ("item_height", self.setting_item_height, self.file_item_height_edit.text()),
        ]

        for key, original_value, new_value in setting_entries:
            if new_value != original_value:
                self.settings.set_setting(key, new_value)

        self.accept()