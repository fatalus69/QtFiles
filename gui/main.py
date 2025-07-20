import sys, os
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QListWidget, QLabel, QListWidgetItem, QHBoxLayout, QLineEdit
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt, QTimer
import fileops

class FileExplorer(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("QtFiles")
        self.setFixedSize(800, 400)
        layout = QVBoxLayout(self)
        
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
        layout.addWidget(self.list_widget)
        
        self.files_to_render = []
        
        self.load_files(self.current_path)
        
        self.list_widget.itemDoubleClicked.connect(self.on_item_double_clicked)

    def load_files(self, fp):
        if not isinstance(fp, str):
            fp = fileops.get_home_directory()
        
        self.current_path = fp
        self.directory_display.setText(self.current_path)
        self.list_widget.clear()
        
        files = fileops.list_files(fp, False)
        self.files_to_render = []
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

            icon_label = QLabel()
            icon_label.setPixmap(self.icon_cache[icon_to_load])
            icon_label.setFixedSize(22, 22)
            icon_label.setScaledContents(True)

            file_layout.addWidget(icon_label)
            file_layout.addWidget(filename_label)

            item = QListWidgetItem()
            item.setSizeHint(file_widget.sizeHint())
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
        print(fileops_result)
        
        self.list_widget.clear()
        # prevent re-rendering of files
        self.files_to_render = []
        self.files_to_render = fileops_result

        self.render_next_batch()

app = QApplication(sys.argv)
window = FileExplorer()
window.show()
sys.exit(app.exec_())
