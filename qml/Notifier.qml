import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Column {
    id: control

    property var notifier
    property bool newNotifier: false
    property bool error: preview.error || exp.optionsError

    signal addedNew
    signal deleted

    NotifierPreview {
        id: preview
        notifier: control.notifier
        newNotifier: control.newNotifier
        onCustomClicked: exp.show = !exp.show
        optionsItem.opacity: exp.show ? 1 : 0
        optionsAux: true

        onDeleted: deleteDialog.open()
    }

    Item {
        height: 3
        width: 1
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        visible: !control.newNotifier
        height: 1
        color: '#bbb'
    }

    Expandable {
        id: exp

        property bool optionsError: false

        onHeightChanged: {
            if (exp.height === 0)
                preview.optionsAux = true
        }

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

            onErrorChanged: {
                if (options.error) {
                    notifier.toastEnabled = false
                    notifier.soundEnabled = false
                }
                exp.optionsError = options.error
            }

            onCanceled: {
                exp.show = false
                preview.cancelNew()
                notifier.reset()
            }

            onAddedNew: {
                if (!preview.isValid())
                    return

                control.addedNew()
                control.newNotifier = false
                preview.finishNew()
                preview.optionsAux = false
                exp.show = !exp.show
                root.saveEnabled = true
            }

            onNewNotifierChanged: control.newNotifier = options.newNotifier
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

            TextMetrics {
                id: metrics
                text: notifier.name
                elide: Qt.ElideRight
                elideWidth: 300
            }

            Label {
                Layout.topMargin: 10
                Layout.preferredWidth: parent.width
                text: "Are you sure you want to delete <b>\"" + metrics.elidedText + "\"</b>?"
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

                    onClicked: control.deleted()
                }
            }
        }
    }
}
