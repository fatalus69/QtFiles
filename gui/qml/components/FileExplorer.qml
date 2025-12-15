import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "QtFiles"

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            Layout.fillWidth: true

            TextField {
                id: directory_display
                Layout.fillWidth: true
                onAccepted: {
                }
            }

            TextField {
                id: search_bar
                Layout.preferredWidth: 200
                placeholderText: "Search for file"
                onAccepted: {}
            }
        }

        // Tree ?
        TableView {
            id: tree_widget
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            columnSpacing: 1
            rowSpacing: 1
        }
    }
}
