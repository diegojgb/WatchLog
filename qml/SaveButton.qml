import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: saveRect

    height: 35
    Layout.preferredHeight: saveRect.height

    radius: 3
    state: mouseArea.containsMouse ? "hoveredState" : "normalState"

    states: [
        State {
            name: "normalState"

            PropertyChanges {
                target: saveRect
                color: root.saveEnabled ? "#005ba1" : "#404040"
            }
        },
        State {
            name: "hoveredState"

            PropertyChanges {
                target: saveRect
                color: mouseArea.pressed ? "#004588" : root.saveEnabled ? "#2e70b8" : "#404040"
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
        anchors.leftMargin: 12
        height: parent.height
        spacing: 8

        Image {
            anchors.verticalCenter: parent.verticalCenter
            source: root.saveEnabled ? "qrc:/assets/save.png" : "qrc:/assets/save-disabled.png"
        }

        Text {
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            text: "Save changes"
            color: root.saveEnabled ? "#fff" : "#ababab"
            font.pointSize: 9
            renderType: Text.NativeRendering
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: root.saveEnabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        hoverEnabled: true
        enabled: root.saveEnabled
        onClicked: {
            Manager.updateJSON()
            root.saveEnabled = false
        }
    }
}
