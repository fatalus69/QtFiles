from PyQt5.QtWidgets import QAction
from PyQt5.QtGui import QKeySequence

class Actions:
    def __init__(self, file_explorer):
        self.file_explorer = file_explorer
        self.actions = {}
        
        self.create_actions()
        self.register_actions()
        
    def create_actions(self):
        refresh_action = QAction("Refresh", self.file_explorer)
        refresh_action.setShortcut(QKeySequence("F5"))
        refresh_action.triggered.connect(self.file_explorer.refresh)

        self.actions = {
            "refresh": refresh_action,
        }
        
    def register_actions(self):
        for action in self.actions.values():
            self.file_explorer.addAction(action)
        
    def get(self, name):
        return self.actions.get(name)    