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
    property int errorCount: 0
    property CustomDialog deleteDialog: deleteDialogItem
    property CustomDialog renameDialog: renameDialogItem
    property bool fileError: monitor.fileError

    signal deleted
    signal rightClicked(Notifier notifier)

    onFileErrorChanged: {
        if (page.fileError)
            monitor.enabled = false
    }

    Flickable {
        anchors.fill: parent
        contentHeight: content.height + content.anchors.topMargin
                       + content.anchors.bottomMargin + page.custBottomPadding
        boundsBehavior: Flickable.StopAtBounds
        focusPolicy: Qt.ClickFocus

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
                        text: "Rename"
                        first: true
                        onTriggered: renameDialogItem.open()
                    }

                    CustomMenuItem {
                        text: "Settings"
                        onTriggered: settingsDialog.open()
                    }

                    CustomMenuItem {
                        text: "Delete"
                        last: true
                        textItem.color: "#ff0000"
                        textItem.anchors.top: textItem.parent.top
                        textItem.anchors.bottom: textItem.parent.bottom
                        textItem.anchors.bottomMargin: 3

                        onTriggered: deleteDialogItem.open()
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
                id: enabledSwitch
                Layout.topMargin: 17
                Layout.leftMargin: -5
                backgroundColor_on: root.accentColor
                checked: monitor.enabled
                font.pointSize: 10
                enabled: monitor.enabledNotifierCount && !page.fileError
                text: "Enabled"

                MouseArea {
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill: parent
                    onClicked: {
                        var init = monitor.enabled

                        monitor.enabled = !enabledSwitch.checked

                        if (init !== monitor.enabled)
                            root.saveEnabled = true
                    }
                }
            }

            FileBrowser {
                Layout.topMargin: 15
                Layout.rightMargin: 10
                filePath: monitor.filePath
                error: page.fileError

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
                    id: notifier

                    Layout.fillWidth: true
                    newNotifier: model.index === monitor.notifiers.rowCount(
                                     ) - 1
                    notifier: model.display

                    onAddedNew: monitor.addEmptyNotifier()
                    onDeleted: monitor.removeNotifier(model.index)
                    onErrorChanged: page.errorCount += error ? 1 : -1
                    onRightClicked: page.rightClicked(notifier)
                }
            }
        }
    }

    CustomDialog {
        id: deleteDialogItem

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

            TextMetrics {
                id: metrics
                text: monitor.name
                elide: Qt.ElideRight
                elideWidth: 150
            }

            Label {
                Layout.topMargin: 10
                Layout.preferredWidth: parent.width
                text: "This will permanently delete <b>\"" + metrics.elidedText
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
                    onClicked: deleteDialogItem.close()
                }

                CustomButton {
                    colorPreset: CustomButton.Color.Red
                    text: "Delete"

                    onClicked: page.deleted()
                }
            }
        }
    }

    CustomDialog {
        id: renameDialogItem
        height: 190

        ColumnLayout {
            anchors.top: parent.top
            anchors.topMargin: 14
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12

            Label {
                text: "Edit"
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
                text: monitor.name

                onEditingFinished: {
                    name.error = name.text === ""
                }
            }

            Row {
                Layout.topMargin: 30
                Layout.alignment: Qt.AlignRight
                spacing: 5

                CustomButton {
                    text: "Cancel"
                    onClicked: renameDialogItem.close()
                }

                CustomButton {
                    colorPreset: CustomButton.Color.Blue
                    text: "Save"

                    onClicked: {
                        if (name.text === "") {
                            name.error = true
                            return
                        }

                        if (monitor.name !== name.text) {
                            monitor.name = name.text
                            root.saveEnabled = true
                        }

                        renameDialogItem.close()
                    }
                }
            }
        }
    }

    CustomDialog {
        id: settingsDialog
        height: 260
        width: 370

        ColumnLayout {
            anchors.top: parent.top
            anchors.topMargin: 18
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12

            Label {
                text: "Monitor settings"
                renderType: Text.NativeRendering
                font.pointSize: 14
                font.bold: true
            }

            Label {
                Layout.topMargin: 18
                Layout.preferredWidth: parent.width
                text: "Default image"
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.DemiBold
            }

            FileBrowser {
                id: imageBrowser
                filePath: monitor.defaultImage
                fileDialog.nameFilters: ["Image files (*.jpg *.jpeg *.png)"]
                error: monitor.imageError
            }

            Label {
                Layout.topMargin: 10
                Layout.preferredWidth: parent.width
                text: "Default sound"
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.DemiBold
            }

            FileBrowser {
                id: soundBrowser
                filePath: monitor.defaultSound
                fileDialog.nameFilters: ["WAV files (*.wav)"]
                error: monitor.soundError
            }

            Row {
                Layout.topMargin: 30
                Layout.alignment: Qt.AlignRight
                spacing: 5

                CustomButton {
                    text: "Cancel"
                    onClicked: settingsDialog.close()
                }

                CustomButton {
                    colorPreset: CustomButton.Color.Blue
                    text: "Save"

                    onClicked: {
                        monitor.defaultSound = soundBrowser.filePath
                        monitor.defaultImage = imageBrowser.filePath

                        settingsDialog.close()
                    }
                }
            }
        }
    }
}
