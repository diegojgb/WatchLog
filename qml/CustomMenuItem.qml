import QtQuick
import QtQuick.Controls

MenuItem {
    id: control

    highlighted: control.hovered

    property Text textItem: text
    property Rectangle backgroundItem: bg
    property bool first: false
    property bool last: false
    property int radius: 4
    property color hoverColor: control.dark ? "#4e4f52" : "#eee"
    property bool dark: false

    contentItem: Text {
        id: text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
        color: control.dark ? "#fff" : "#000"
        leftPadding: 5
        text: control.text
        font: control.font
    }

    background: Rectangle {
        id: bg
        implicitWidth: 200
        implicitHeight: 27
        color: control.highlighted ? control.hoverColor : "transparent"
        topLeftRadius: control.first ? control.radius : 0
        topRightRadius: control.first ? control.radius : 0
        bottomLeftRadius: control.last ? control.radius : 0
        bottomRightRadius: control.last ? control.radius : 0
    }
}
