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
        spacing: 5

        Label {
            text: monitor.name
            font.pointSize: 20
            font.bold: true
            renderType: Text.NativeRendering
        }

        RowLayout {
            Layout.topMargin: 24
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
                    color: "#e6e6e6"
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

        Row {
            spacing: 20
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 20
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
                    height: 1
                    color: '#bbb'
                    Layout.fillWidth: true
                }
            }
        }
    }
}
