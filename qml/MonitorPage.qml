import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

ScrollView {
    id: page
    Layout.fillHeight: true
    Layout.fillWidth: true

    property var monitor

    Flickable {
        anchors.fill: parent
        contentHeight: content.height + content.anchors.topMargin + content.anchors.bottomMargin
        boundsBehavior: Flickable.StopAtBounds

        // Makes widgets lose focus when clicked outside.
        Pane {
            anchors.fill: parent
            focusPolicy: Qt.ClickFocus
        }

        ColumnLayout {
            id: content

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 25
            anchors.topMargin: 30
            anchors.rightMargin: 28

            Label {
                Layout.fillWidth: true
                text: monitor.name
                font.pointSize: 20
                font.bold: true
                elide: Text.ElideRight
                maximumLineCount: 1
                renderType: Text.NativeRendering

                ToolTip.text: monitor.name
                ToolTip.visible: truncated && mouseArea.containsMouse
                ToolTip.delay: 1000
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                }
            }

            CusSwitch {
                Layout.topMargin: 17
                Layout.leftMargin: -5
                backgroundColor_on: root.accentColor
                checked: monitor.enabled
                font.pointSize: 10
                text: "Enabled"

                property bool loaded: false
                Component.onCompleted: loaded = true

                onCheckedChanged: {
                    monitor.enabled = checked
                    if (loaded)
                        root.saveEnabled = true
                }
            }

            FileBrowser {
                Layout.topMargin: 15
                Layout.rightMargin: 10
                filePath: monitor.filePath

                onFileAccepted: {
                    monitor.filePath = filePath
                    root.saveEnabled = true
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
                            custBottomMargin: 12
                            notifier: monitor.nListAt(modelData)
                        }
                    }
                }
            }

            // // Add notification button
            // Rectangle {
            //     Layout.topMargin: 15
            //     width: 140
            //     height: 30
            //     radius: 2
            //     border.width: 1
            //     border.color: ma.containsMouse ? "#0078d4" : "#ababab"
            //     color: ma.pressed ? "#d0ffff" : "#fff"

            //     Behavior on border.color {
            //         ColorAnimation {
            //             duration: 200
            //         }
            //     }

            //     Row {
            //         anchors.fill: parent
            //         anchors.leftMargin: 8
            //         spacing: 7

            //         Text {
            //             anchors.bottom: parent.bottom
            //             anchors.bottomMargin: 1
            //             renderType: Text.NativeRendering
            //             font.pixelSize: 26
            //             text: '+'
            //         }

            //         Text {
            //             anchors.verticalCenter: parent.verticalCenter
            //             renderType: Text.NativeRendering
            //             text: "Add notification"
            //         }
            //     }

            //     MouseArea {
            //         id: ma
            //         anchors.fill: parent
            //         hoverEnabled: true

            //         onClicked: customDialog.open()
            //     }
            // }
            Item {
                height: 40
                width: 1
            }
        }
    }
}
