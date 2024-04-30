import QtQuick
import QtQuick.Controls

Rectangle {
    id: control
    color: control.fillColor
    radius: 3
    height: 35
    width: childrenRect.width

    property color textColor: "#fff"
    property color fillColor: "#005ba1"
    property color hoverColor: Qt.lighter(control.fillColor, 1.3)
    property color pressedColor: Qt.darker(control.fillColor, 1.2)

    property Text textIconItem: textIcon
    property Text textItem: mainText

    property int horizontalPadding: 12
    property alias text: mainText.text

    signal clicked

    state: mouseArea.containsMouse ? "hoveredState" : "normalState"

    states: [
        State {
            name: "normalState"

            PropertyChanges {
                target: control
                color: control.fillColor
            }
        },
        State {
            name: "hoveredState"

            PropertyChanges {
                target: control
                color: mouseArea.pressed ? control.pressedColor : control.hoverColor
            }
        }
    ]

    transitions: Transition {
        ColorAnimation {
            duration: root.transitionDuration
        }
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        anchors.left: parent.left
        anchors.leftMargin: control.horizontalPadding
        anchors.rightMargin: 12
        height: parent.height
        spacing: textIcon.visible ? 8 : 0

        Text {
            id: textIcon
            visible: false // off by default.
            color: "#fff"
            renderType: Text.NativeRendering
            verticalAlignment: Text.AlignVCenter
            height: parent.height
            bottomPadding: 4
            font.pixelSize: 26
            text: '+'
        }

        Text {
            id: mainText
            color: control.textColor
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 11
            renderType: Text.NativeRendering
        }

        Item {
            id: rightPaddingItem
            height: 1
            width: control.horizontalPadding
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onClicked: control.clicked()
    }
}
