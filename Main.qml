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
            Layout.preferredWidth: 190
            color: '#2d2e30'

            ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                Row {
                    Layout.topMargin: 15
                    Layout.leftMargin: 14
                    spacing: 8

                    Image {
                        id: logo
                        source: "assets/watchlog-logo.png"
                    }
                    Label {
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: "WatchLog"
                        color: "#fff"
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.topMargin: 8
                    Layout.rightMargin: 12
                    Layout.leftMargin: 12
                    height: 1
                    color: '#444'
                }

                Label {
                    Layout.topMargin: 1
                    Layout.leftMargin: 12
                    text: "Monitors"
                    color: '#888'
                }

                Column {
                    id: tabBar
                    Layout.fillWidth: true
                    Layout.topMargin: 10

                    property int tabIndex: 0

                    Repeater {
                        id: tabRepeater

                        model: Manager.monitorsOrder

                        Tab {
                            width: parent.width
                            height: 30
                            text: Manager.hashGet(modelData).name
                            checked: tabBar.tabIndex == index

                            property int index: tabRepeater.model.indexOf(
                                                    modelData)

                            onClicked: {
                                tabBar.tabIndex = tabRepeater.model.indexOf(
                                            modelData)
                            }
                        }
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