import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

TabButton {
    id: control

    property alias contentAnchors: content.anchors
    property color textColor: "#fff"
    property bool selected
    property bool enabled
    property alias truncated: text.truncated

    signal rightClicked

    checked: control.selected
    font.pointSize: 10

    background: Rectangle {
        id: background
        anchors.fill: parent
        radius: 7

        color: {
            if (control.hovered) {
                return "#4d4d4d"
            } else if (control.checked) {
                return "#404040"
            } else {
                return "#2d2e30"
            }
        }

        Behavior on color {
            ColorAnimation {
                duration: root.transitionDuration
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

    contentItem: Item {
        anchors.fill: background

        RowLayout {
            id: content
            spacing: 5
            anchors.fill: parent
            anchors.bottomMargin: 4

            Text {
                id: text
                Layout.fillWidth: true
                Layout.leftMargin: 12
                topPadding: 2
                text: control.text
                font: control.font
                color: control.textColor
                elide: Text.ElideRight
                maximumLineCount: 1
                renderType: Text.NativeRendering
            }

            Item {
                Layout.fillWidth: true
            }

            Rectangle {
                Layout.rightMargin: 13
                Layout.topMargin: 3
                width: 9
                height: 9
                radius: width / 2
                color: control.enabled ? "#009f00" : "#dd0000"
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true

        onClicked: mouse => {
                       if (mouse.button === Qt.LeftButton) {
                           control.clicked()
                       } else if (mouse.button === Qt.RightButton) {
                           control.rightClicked()
                       }
                   }
    }
}
