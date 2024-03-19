import QtQuick
import QtQuick.Controls.Fusion

Button {
    id: control
    font.pointSize: 8

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        renderType: Text.NativeRendering
    }

    background: Rectangle {
        anchors.fill: parent
        opacity: enabled ? 1 : 0.3
        color: control.pressed ? "#ddd" : "#fff"
        border.color: control.hovered ? "#555" : "#ababab"
        border.width: 1
        radius: 2
    }
}
