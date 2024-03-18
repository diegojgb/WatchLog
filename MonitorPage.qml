import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import TaoQuick 1.0

Item {
    id: page

    property var monitor

    Layout.fillHeight: true
    Layout.fillWidth: true

    ColumnLayout {
        id: content

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.topMargin: 30
        anchors.rightMargin: 28

        Label {
            text: monitor.name
            font.pointSize: 20
            font.bold: true
            renderType: Text.NativeRendering
        }

        CusSwitch {
            Layout.topMargin: 17
            Layout.leftMargin: -5
            backgroundColor_on: root.accentColor
            checked: monitor.enabled
            text: "Enabled"

            onCheckedChanged: {
                monitor.enabled = checked
            }
        }

        FileBrowser {
            Layout.topMargin: 15
            Layout.rightMargin: 10
            filePath: monitor.filePath
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

            Column {
                Layout.fillWidth: true

                NotifierPreview {
                    notifier: monitor.nListAt(modelData)
                    onCustomClicked: exp.show = !exp.show
                }

                Item {
                    height: 3
                    width: 1
                }

                Rectangle {
                    visible: modelData < monitor.nListLength() - 1
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: '#bbb'
                }

                Expandable {
                    id: exp

                    NotifierOptions {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: 10
                        anchors.rightMargin: 22
                        cusTopMargin: 20
                        custBottomMargin: 10
                        notifier: monitor.nListAt(modelData)
                    }
                }
            }
        }
    }
}
