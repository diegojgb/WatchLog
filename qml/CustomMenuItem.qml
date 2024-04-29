import QtQuick
import QtQuick.Controls

MenuItem {
    id: control

    highlighted: control.hovered

    property Text textItem: text
    property Rectangle backgroundItem: bg

    contentItem: Text {
        id: text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
        leftPadding: 5
        text: control.text
        font: control.font
        color: "#ff0000"
    }

    background: Rectangle {
        id: bg
        implicitWidth: 200
        implicitHeight: 27
        color: control.highlighted ? "#eee" : "transparent"
        radius: 4
    }
}
