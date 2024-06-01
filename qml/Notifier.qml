import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Column {
    id: control

    property var notifier
    property bool monitorEnabled
    property int index
    property bool newNotifier: false
    property bool error: !control.newNotifier && (preview.error
                                                  || notifier.regexError
                                                  || notifier.soundFileError
                                                  || notifier.imageFileError)

    signal addedNew
    signal deleted
    signal rightClicked

    function rename() {
        preview.rename()
    }

    onErrorChanged: {
        if (control.error) {
            notifier.toastEnabled = false
            notifier.soundEnabled = false
        }
    }

    NotifierPreview {
        id: preview
        notifier: control.notifier
        newNotifier: control.newNotifier
        onCustomClicked: exp.show = !exp.show
        optionsItem.opacity: exp.show ? 1 : 0
        exclamationItem.opacity: exp.show ? 0 : 1
        notifierError: control.error
        monitorEnabled: control.monitorEnabled

        onDeleted: control.deleted()
        onRightClicked: {
            if (!control.newNotifier)
                control.rightClicked()
        }
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
            notifierError: control.error

            onCanceled: {
                exp.show = false
                preview.cancelNew()
                notifier.reset()
            }

            onAddedNew: {
                if (preview.textFieldError)
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
}
