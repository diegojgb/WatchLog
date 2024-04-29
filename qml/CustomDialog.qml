import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: control
    anchors.centerIn: Overlay.overlay
    modal: true
    width: 300

    // Makes widgets lose focus when clicked outside.
    Overlay.modal: Rectangle {
        color: Qt.rgba(0, 0, 0, 0.4)
    }

    background: Pane {
        anchors.fill: parent
        focusPolicy: Qt.ClickFocus

        background: Rectangle {
            anchors.fill: parent
            color: root.whiteColor
            radius: 7
        }
    }
}
