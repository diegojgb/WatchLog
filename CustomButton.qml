import QtQuick
import QtQuick.Controls.Fusion

Button {
    id: control
    font.pointSize: 8

    property color bgColor: control.pressed ? "#ddd" : "#fff"

    contentItem: Text {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        bottomPadding: 2
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        elide: Text.ElideRight
        renderType: Text.NativeRendering
    }

    background: Rectangle {
        id: bg
        anchors.fill: parent
        opacity: enabled ? 1 : 0.3
        color: control.bgColor
        border.color: control.hovered ? "#555" : "#ababab"
        border.width: 1
        radius: 2
    }
}
