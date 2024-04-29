import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root

    visible: true
    title: qsTr("WatchLog")

    width: 841
    height: 560

    property color accentColor: "#0078d4"
    property color whiteColor: "#fbfbfb"
    property int transitionDuration: 150
    property bool saveEnabled: false

    // Hide the window instead of closing it when the close button is clicked
    // @disable-check M16
    onClosing: close => {
                   hide()
                   close.accepted = false
               }

    // Makes widgets lose focus when clicked outside.
    Pane {
        anchors.fill: parent
        focusPolicy: Qt.ClickFocus
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Sidebar
        Sidebar {
            id: sidebar
            Layout.fillHeight: true
            Layout.preferredWidth: 190
            color: '#2d2e30'
        }

        // Separator
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: "#2b2b2b"
        }

        // Mainbar
        Rectangle {
            id: mainbar
            Layout.fillHeight: true
            Layout.fillWidth: true

            StackLayout {
                id: stackView
                anchors.fill: parent
                currentIndex: sidebar.tabBar.tabIndex

                Repeater {
                    model: Manager.monitors

                    MonitorPage {
                        monitor: model.edit

                        onDeleted: {
                            if (sidebar.tabBar.tabIndex !== 0)
                                sidebar.tabBar.tabIndex = sidebar.tabBar.tabIndex - 1

                            Manager.monitors.remove(model.edit.filePath)
                        }
                    }
                }

                HomePage {}
            }
        }
    }
}
