import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Column {
    id: control

    required property var notifier
    property bool newNotifier: false

    NotifierPreview {
        notifier: control.notifier
        newNotifier: control.newNotifier
        onCustomClicked: exp.show = !exp.show
        optionsItem.opacity: exp.show ? 1 : 0
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
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 22
            newNotifier: control.newNotifier
            cusTopMargin: 20
            custBottomMargin: 13
            notifier: control.notifier

            onCanceled: exp.show = false
        }
    }
}
