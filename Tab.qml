import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

TabButton {
    id: control

    property alias contentAnchors: content.anchors
    property alias imageSource: image.source
    property color textColor: "#fff"
    property bool selected

    checked: control.selected
    font.pointSize: 10

    background: Rectangle {
        id: background
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        radius: 7

        color: {
            if (control.hovered) {
                return "#4d4d4d"
            } else if (control.checked) {
                return "#404040"
            } else {
                return "transparent"
            }
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            height: 16
            width: 3
            radius: 10
            color: root.accentColor
            visible: control.selected
        }
    }

    contentItem: Row {
        id: content
        spacing: 6
        anchors.fill: parent
        anchors.leftMargin: 20

        Image {
            anchors.verticalCenter: parent.verticalCenter
            id: image
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            bottomPadding: 3
            text: "»"
            font.pointSize: 14
            color: control.textColor
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: control.text
            font: control.font
            color: control.textColor
            renderType: Text.NativeRendering
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onPressed: mouse => {
                       mouse.accepted = false
                   }
    }
}
