import sys, os
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QListWidget, QLabel, QListWidgetItem, QHBoxLayout, QLineEdit
from file_explorer import FileExplorer

app = QApplication(sys.argv)
window = FileExplorer()
window.show()
sys.exit(app.exec_())
