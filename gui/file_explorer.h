#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QTreeWidget>

#include <QPixmap>
#include <QTimer>
#include <QSize>
#include <string>

class FileExplorer : public QWidget 
{
public:
  FileExplorer();

private slots:
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onDirectoryEntered();
    void onSearchEntered();
    void openSettings();

private:
    void initUI();
    void loadFiles(std::string &path);

    QVBoxLayout *mainLayout;
    QHBoxLayout *topBar;

    QMenuBar *menuBar;
    QLineEdit *directoryDisplay;
    QLineEdit *search_bar;
    QTreeWidget *treeWidget;

    QMap<QString, QPixmap> icon_cache;

    std::string current_path;
    QList<QVariant> filesToRender; // Placeholder — replace with your file object type
};
