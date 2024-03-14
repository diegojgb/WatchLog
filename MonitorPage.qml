import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import TaoQuick 1.0

Item {
    property var monitor

    Layout.fillHeight: true
    Layout.fillWidth: true

    ColumnLayout {
        id: page

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.topMargin: 30
        anchors.rightMargin: 28
        spacing: 3

        Label {
            text: monitor.name
            font.pointSize: 20
            font.bold: true
            renderType: Text.NativeRendering
        }

        CusSwitch {
            Layout.topMargin: 16
            Layout.leftMargin: -5
            backgroundColor_on: root.accentColor
            checked: monitor.enabled
            text: "Enabled"

            onCheckedChanged: {
                monitor.enabled = checked
            }
        }

        RowLayout {
            Layout.topMargin: 16
            Layout.rightMargin: 10

            FileDialog {
                id: fileDialog
                title: "Select a file"
                selectedFile: "file:///" + monitor.filePath
                onSelectedFileChanged: {
                    monitor.filePath = selectedFile.toString().slice(8)
                }
            }

            TextField {
                id: pathField
                Layout.fillWidth: true
                Layout.preferredHeight: 25
                enabled: false
                text: monitor.filePath
                verticalAlignment: Text.AlignVCenter
                leftPadding: 5
                bottomPadding: 4
                placeholderText: "No file selected..."
                renderType: Text.NativeRendering

                background: Rectangle {
                    color: "#e0e0e0"
                    border.color: "#bbb"
                }
            }

            Button {
                Layout.preferredWidth: 80
                Layout.preferredHeight: 26
                text: "Browse..."
                onClicked: fileDialog.open()
            }
        }

        Label {
            Layout.topMargin: 30
            text: "Notifications"
            font.pointSize: 14
            font.weight: Font.DemiBold
            renderType: Text.NativeRendering
        }

        Row {
            spacing: 20
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 8

            Label {
                text: "TOAST"
                font.pointSize: 10.5
                font.bold: true
            }
            Label {
                text: "SOUND"
                font.pointSize: 10.5
                font.bold: true
            }
        }

        Repeater {
            model: monitor.nListLength()

            ColumnLayout {
                Layout.fillWidth: true

                RowLayout {
                    width: parent.width

                    Text {
                        Layout.leftMargin: 5
                        font.pointSize: 10
                        renderType: Text.NativeRendering
                        text: monitor.nListAt(modelData).name
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Row {
                        spacing: 8

                        CusSwitch {
                            checked: monitor.nListAt(modelData).toastEnabled
                            backgroundColor_on: root.accentColor
                            onCheckedChanged: {
                                monitor.nListAt(
                                            modelData).toastEnabled = checked
                            }
                        }
                        CusSwitch {
                            checked: monitor.nListAt(modelData).soundEnabled
                            backgroundColor_on: root.accentColor
                            onCheckedChanged: {
                                monitor.nListAt(
                                            modelData).soundEnabled = checked
                            }
                        }
                    }
                }
                Rectangle {
                    visible: modelData < monitor.nListLength() - 1
                    height: 1
                    color: '#bbb'
                    Layout.fillWidth: true
                }
            }
        }
    }
}
