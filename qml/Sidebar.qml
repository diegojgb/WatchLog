import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: control

    property var tabBar: tabBarItem

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right

        Row {
            Layout.topMargin: 18
            Layout.leftMargin: 14
            spacing: 8

            Image {
                id: logo
                source: "qrc:/assets/watchlog-logo.png"
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

        Label {
            Layout.topMargin: 12
            Layout.leftMargin: 12
            text: "Monitors"
            color: '#999'
        }

        ColumnLayout {
            id: tabBarItem
            Layout.fillWidth: true
            Layout.topMargin: 8

            property int tabIndex: 0

            Repeater {
                id: tabRepeater
                model: Manager.monitors

                Tab {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    Layout.leftMargin: 10
                    Layout.rightMargin: 10
                    text: model.display
                    enabled: model.edit.enabled
                    selected: tabBarItem.tabIndex == idx

                    property int idx: model.index

                    onClicked: {
                        tabBarItem.tabIndex = model.index
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.topMargin: 4
                Layout.rightMargin: 12
                Layout.leftMargin: 12
                height: 1
                color: '#444'
            }

            // Add monitor button
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                color: mouseArea.containsMouse ? "#4d4d4d" : "#2d2e30"
                radius: 7

                Behavior on color {
                    ColorAnimation {
                        duration: root.transitionDuration
                    }
                }

                Row {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    spacing: 5

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/assets/add-new.png"
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        renderType: Text.NativeRendering
                        font.pointSize: 10
                        color: "white"
                        text: 'Add monitor'
                    }
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onClicked: addMonitorDialog.open()
                }
            }
        }
    }

    SaveButton {}

    Dialog {
        id: addMonitorDialog
        anchors.centerIn: Overlay.overlay
        modal: true
        width: 300
        height: 250

        // Makes widgets lose focus when clicked outside.
        Pane {
            anchors.fill: parent
            focusPolicy: Qt.ClickFocus
        }

        Overlay.modal: Rectangle {
            color: Qt.rgba(0, 0, 0, 0.4) // Use whatever color/opacity you like
        }

        background: Rectangle {
            height: addMonitorDialog.height
            width: addMonitorDialog.width
            radius: 7
        }

        ColumnLayout {
            id: dialogColumn
            anchors.top: parent.top
            anchors.topMargin: 14
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12

            Label {
                text: "Add monitor"
                renderType: Text.NativeRendering
                font.pointSize: 14
                font.bold: true
            }

            Label {
                Layout.topMargin: 14
                Layout.preferredWidth: parent.width
                text: "Name"
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.DemiBold
            }

            CustomTextField {
                id: name
                Layout.fillWidth: true
                placeholderText: "Enter monitor name..."
            }

            Label {
                Layout.topMargin: 10
                Layout.preferredWidth: parent.width
                text: "File path"
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.DemiBold
            }

            FileBrowser {
                id: fileBrowser
                // fileDialog.nameFilters: ["Image files (*.jpg *.jpeg *.png)"
            }

            Row {
                Layout.topMargin: 30
                Layout.alignment: Qt.AlignRight
                spacing: 5

                CustomButton {
                    width: 80
                    text: "Cancel"

                    onClicked: addMonitorDialog.close()
                }

                CustomButton {
                    width: 80
                    bgItem.color: pressed ? "#d0ffff" : root.whiteColor
                    bgItem.border.color: hovered ? "#44a9ff" : "#0078d4"
                    textItem.color: "#0078d4"
                    text: "Add"

                    onClicked: {
                        Manager.addMonitor(name.text, fileBrowser.selectedFile)
                        tabBarItem.tabIndex = tabRepeater.count - 1
                        addMonitorDialog.close()
                    }
                }
            }
        }
    }
}
