import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: control

    property var tabBar: tabBarItem

    signal tabRightClicked(int tabIndex)

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

                    onClicked: tabBarItem.tabIndex = model.index
                    onRightClicked: tabRightClicked(idx)
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

    SaveButton {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
        anchors.bottomMargin: 25
    }

    CustomDialog {
        id: addMonitorDialog
        height: 250

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

                onEditingFinished: name.error = name.text === ""
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

                fileDialog.onAccepted: {
                    fileBrowser.error = false
                }
            }

            Row {
                Layout.topMargin: 30
                Layout.alignment: Qt.AlignRight
                spacing: 5

                CustomButton {
                    text: "Cancel"
                    onClicked: addMonitorDialog.close()
                }

                CustomButton {
                    colorPreset: CustomButton.Color.Blue
                    text: "Add"

                    onClicked: {
                        if (name.text === "") {
                            name.error = true
                            return
                        }
                        if (fileBrowser.filePath === "") {
                            fileBrowser.error = true
                            return
                        }

                        Manager.addMonitor(name.text, fileBrowser.filePath)
                        tabBarItem.tabIndex = tabRepeater.count - 1
                        addMonitorDialog.close()
                    }
                }
            }
        }
    }
}
