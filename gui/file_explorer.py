from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QLineEdit, QHBoxLayout, QMessageBox,
    QListWidget, QListWidgetItem, QLabel, QMenuBar, QAction, QDialog
)
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt, QTimer, QSize
from settings import Settings
import fileops, os

class FileExplorer(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("QtFiles")
        self.setFixedSize(800, 400)
        self.settings = fileops.Settings()
        self.init_ui()
        self.load_files(self.current_path)

    def init_ui(self):
        layout = QVBoxLayout(self)
        
        self.menubar = QMenuBar(self)
        settings_menu = self.menubar.addMenu("File")

        open_settings_action = QAction("Settings", self)
        open_settings_action.triggered.connect(self.open_settings)
        settings_menu.addAction(open_settings_action)
        
        layout.setMenuBar(self.menubar)

        self.icon_cache = {
            "folder_filled": QPixmap("gui/assets/folder_filled.png"),
            "folder_empty": QPixmap("gui/assets/folder_empty.png"),
            "file": QPixmap("gui/assets/file.png")
        }

        self.current_path = fileops.get_home_directory()

        self.directory_display = QLineEdit()
        self.directory_display.setText(self.current_path)
        self.directory_display.returnPressed.connect(self.on_directory_entered)

        self.search_bar = QLineEdit()
        self.search_bar.setFixedWidth(200)
        self.search_bar.setPlaceholderText("Search")
        self.search_bar.returnPressed.connect(self.on_search_enter)

        self.top_bar = QHBoxLayout()
        self.top_bar.addWidget(self.directory_display)
        self.top_bar.addWidget(self.search_bar)

        layout.addLayout(self.top_bar)

        self.list_widget = QListWidget()
        self.list_widget.setStyleSheet(f"QListWidget::item:selected {{ background-color: {self.settings.get_setting('selected_bg_color')}; }}")

        layout.addWidget(self.list_widget)
        
        self.files_to_render = []
        self.list_widget.itemDoubleClicked.connect(self.on_item_double_clicked)

    def load_files(self, fp):
        if not isinstance(fp, str):
            fp = fileops.get_home_directory()

        self.current_path = fp
        self.directory_display.setText(self.current_path)
        self.list_widget.clear()

        files = fileops.list_files(fp, False)
        self.files_to_render = files
        self.render_next_batch()

    def render_next_batch(self):
        BATCH_SIZE = 100
        count = 0

        while self.files_to_render and count < BATCH_SIZE:
            file = self.files_to_render.pop(0)
            file_widget = QWidget()
            file_layout = QHBoxLayout(file_widget)
            filename_label = QLabel(file.filename)

            if file.is_directory:
                if fileops.check_for_dir_contents(os.path.join(self.current_path, file.filename)):
                    icon_to_load = "folder_filled"
                else:
                    icon_to_load = "folder_empty"
            else:
                icon_to_load = "file"

            file_widget = QWidget()
            file_layout = QHBoxLayout(file_widget)
            file_layout.setContentsMargins(4, 2, 4, 2)
            file_layout.setSpacing(6)

            filename_label = QLabel(file.filename)
            filename_label.setStyleSheet(f"font-size: {self.settings.get_setting("font_size")}px;")
                      
            icon_size_int = int(self.settings.get_setting("icon_size"))
            
            icon_label = QLabel()
            icon_label.setPixmap(self.icon_cache[icon_to_load])
            icon_label.setFixedSize(icon_size_int, icon_size_int)
            icon_label.setScaledContents(True)

            file_layout.addWidget(icon_label)
            file_layout.addWidget(filename_label)

            item = QListWidgetItem()
            item.setSizeHint(QSize(0, int(self.settings.get_setting("item_height"))))
            item.setData(Qt.UserRole, os.path.join(self.current_path, file.filename))
            item.setData(Qt.UserRole + 1, file.is_directory)

            self.list_widget.addItem(item)
            self.list_widget.setItemWidget(item, file_widget)

            count += 1

        if self.files_to_render:
            QTimer.singleShot(10, self.render_next_batch)

    def on_item_double_clicked(self, item):
        is_directory = item.data(Qt.UserRole + 1)

        if is_directory:
            self.load_files(item.data(Qt.UserRole))

    def on_directory_entered(self):
        self.load_files(self.directory_display.text())

    def on_search_enter(self):
        search_text = self.search_bar.text()
        fileops_result = fileops.search_in_dir(self.current_path, search_text)

        self.list_widget.clear()
        self.files_to_render = fileops_result
        self.render_next_batch()
        
    def open_settings(self):
        Settings()
        self.load_files(self.current_path)
