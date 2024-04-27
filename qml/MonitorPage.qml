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

    Flickable {
        anchors.fill: parent
        contentHeight: content.height + content.anchors.topMargin + content.anchors.bottomMargin + page.custBottomPadding
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
}
