import QtQuick

Rectangle {
    id: control
    width: parent.width
    height: show ? expanded.height : 0
    color: "#f5f5f5"
    border.color: "#bbb"
    border.width: 1
    clip: true
    bottomLeftRadius: 8
    bottomRightRadius: 8

    default property alias item: expanded.sourceComponent
    property bool show: false

    Loader {
        id: expanded
        width: parent.width
        y: control.height - height
    }

    Rectangle {
        width: parent.width
        height: 1
        color: "#bbb"
    }

    Behavior on height {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
}
