import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs

ScrollView {
    id: page
    Layout.fillHeight: true
    Layout.fillWidth: true

    property var monitor
    property int custBottomPadding: 40

    signal deleted

    Flickable {
        anchors.fill: parent
        contentHeight: content.height + content.anchors.topMargin
                       + content.anchors.bottomMargin + page.custBottomPadding
        boundsBehavior: Flickable.StopAtBounds

        // Makes widgets lose focus when clicked outside.
        Pane {
            anchors.fill: parent
            focusPolicy: Qt.ClickFocus

            background: Rectangle {
                anchors.fill: parent
                color: root.whiteColor
            }
        }

        ColumnLayout {
            id: content

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 25
            anchors.topMargin: 20
            anchors.rightMargin: 28

            Rectangle {
                id: settingsButton
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                Layout.alignment: Qt.AlignRight
                color: settingsButtonMa.containsMouse ? "#ccc" : root.whiteColor
                radius: 4

                Behavior on color {
                    ColorAnimation {
                        duration: root.transitionDuration
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/assets/settings.png"
                }

                MouseArea {
                    id: settingsButtonMa
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {

                        // var x = mouse.x - control.width
                        // var y = mouse.y
                        // control.popup(x, y)
                        if (control.visible) {
                            control.close()
                            return
                        }

                        var x = parent.width - control.width
                        var y = parent.height
                        control.popup(x, y)
                    }
                }

                Menu {
                    id: control
                    closePolicy: Popup.CloseOnPressOutsideParent

                    background: Rectangle {
                        implicitWidth: 150
                        color: "#fff"
                        border.color: "#ababab"
                        radius: 4
                    }

                    CustomMenuItem {
                        text: "Delete"
                        textItem.color: "#ff0000"
                        onTriggered: deleteDialog.open()
                    }
                }
            }

            Label {
                Layout.fillWidth: true
                Layout.rightMargin: settingsButton.width
                Layout.topMargin: -26
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
                Layout.preferredHeight: 19
                Layout.rightMargin: 8

                Label {
                    visible: repeater.count > 1
                    text: "TOAST"
                    font.pointSize: 10.5
                    font.bold: true
                }
                Label {
                    visible: repeater.count > 1
                    text: "SOUND"
                    font.pointSize: 10.5
                    font.bold: true
                }
            }

            Repeater {
                id: repeater
                model: monitor.notifiers

                Notifier {
                    Layout.fillWidth: true
                    newNotifier: model.index === monitor.notifiers.rowCount(
                                     ) - 1
                    notifier: model.display

                    onAddedNew: monitor.addEmptyNotifier()
                    onDeleted: monitor.removeNotifier(model.index)
                }
            }
        }
    }

    CustomDialog {
        id: deleteDialog

        ColumnLayout {
            id: dialogColumn
            anchors.top: parent.top
            anchors.topMargin: 14
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 12

            Label {
                text: "Delete?"
                renderType: Text.NativeRendering
                font.pointSize: 12
                font.bold: true
            }

            Label {
                Layout.topMargin: 10
                Layout.preferredWidth: parent.width
                text: "This will permanently delete <b>\"" + monitor.name
                      + "\"</b>and all its notifications. This can’t be undone."
                wrapMode: Text.Wrap
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
            }

            Row {
                Layout.preferredHeight: 30
                Layout.topMargin: 25
                Layout.alignment: Qt.AlignRight
                Layout.bottomMargin: 20
                spacing: 5

                CustomButton {
                    text: "Cancel"
                    onClicked: deleteDialog.close()
                }

                CustomButton {
                    colorPreset: CustomButton.Color.Red
                    text: "Delete"

                    onClicked: page.deleted()
                }
            }
        }
    }
}
