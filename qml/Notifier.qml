import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Column {
    id: control

    required property var notifier
    property bool newNotifier: false

    signal addedNew
    signal deleted

    NotifierPreview {
        id: preview
        notifier: control.notifier
        newNotifier: control.newNotifier
        onCustomClicked: exp.show = !exp.show
        optionsItem.opacity: exp.show ? 1 : 0

        onDeleted: deleteDialog.open()
    }

    Item {
        height: 3
        width: 1
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        visible: !newNotifier
        height: 1
        color: '#bbb'
    }

    Expandable {
        id: exp

        NotifierOptions {
            id: options
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 22
            newNotifier: control.newNotifier
            cusTopMargin: 20
            custBottomMargin: 13
            notifier: control.notifier

            onCanceled: {
                exp.show = false
                preview.cancelNew()
            }
            onAddedNew: {
                control.addedNew()
                preview.finishNew()
                exp.show = !exp.show
            }
            onNewNotifierChanged: control.newNotifier = options.newNotifier
        }
    }

    Dialog {
        id: deleteDialog
        anchors.centerIn: Overlay.overlay
        modal: true
        width: 300

        Overlay.modal: Rectangle {
            color: Qt.rgba(0, 0, 0, 0.4) // Use whatever color/opacity you like
        }

        background: Rectangle {
            height: deleteDialog.height
            width: deleteDialog.width
            radius: 7
        }

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
                text: "Are you sure you want to delete <b>\"" + notifier.name + "\"</b>?"
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
                    width: 80
                    height: 24
                    text: "Cancel"

                    onClicked: deleteDialog.close()
                }

                CustomButton {
                    width: 80
                    height: 24
                    bgItem.color: pressed ? Qt.rgba(1, 0, 0,
                                                    0.15) : root.whiteColor
                    bgItem.border.color: hovered ? "#ff6b40" : "#ff0000"
                    textItem.color: "#ff0000"
                    text: "Delete"

                    onClicked: control.deleted()
                }
            }
        }
    }
}
