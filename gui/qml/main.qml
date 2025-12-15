import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 800
    height: 400
    visible: true
    title: "QtFiles"

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Settings"
                onTriggered: {}
            }
        }
    }

    FileExplorer {
        anchors.fill: parent
    }
}