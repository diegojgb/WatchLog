import QtQuick

Rectangle {
    id: info
    width: parent.width
    height: show ? expanded.height : 0
    color: "#e6e6e6"
    border.color: "#bbb"
    border.width: 1
    clip: true

    default property alias item: expanded.sourceComponent
    property bool show: false

    Loader {
        id: expanded
        anchors.horizontalCenter: info.horizontalCenter
        y: info.height - height
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
