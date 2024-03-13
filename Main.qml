import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TaoQuick 1.0

ApplicationWindow {
    id: root

    visible: true
    title: qsTr("Hello World")

    width: 840
    height: 560

    property color accentColor: "#0078d4"

    // Makes widgets lose focus when clicked outside.
    Pane {
        anchors.fill: parent
        focusPolicy: Qt.ClickFocus
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Sidebar
        Rectangle {
            id: sidebar

            Layout.fillHeight: true
            Layout.preferredWidth: 180
            color: '#2d2e30'

            Repeater {
                id: tabBar
                anchors.fill: parent

                property int tabIndex: 0

                model: Manager.monitorsOrder

                Tab {
                    width: parent.width
                    height: 48
                    text: Manager.hashGet(modelData).name
                    checked: tabBar.tabIndex == index

                    property int index: tabBar.model.indexOf(modelData)

                    onClicked: {
                        tabBar.tabIndex = tabBar.model.indexOf(modelData)
                    }
                }
            }
        }

        // Mainbar
        Rectangle {
            id: mainbar
            Layout.fillHeight: true
            Layout.fillWidth: true

            StackLayout {
                id: stackView
                anchors.fill: parent
                currentIndex: tabBar.tabIndex

                Repeater {
                    model: Manager.monitorsOrder

                    MonitorPage {
                        monitor: Manager.hashGet(modelData)
                    }
                }
            }
        }
    }
}
