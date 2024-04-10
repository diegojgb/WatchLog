import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

TabButton {
    id: control

    property alias contentAnchors: content.anchors
    property color textColor: "#fff"
    property bool selected
    property bool enabled

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

    contentItem: RowLayout {
        id: content
        spacing: 5
        anchors.fill: background

        Text {
            Layout.fillWidth: true
            Layout.leftMargin: 12
            topPadding: 2
            text: control.text
            font: control.font
            color: control.textColor
            elide: Text.ElideRight
            maximumLineCount: 1
            renderType: Text.NativeRendering

            ToolTip.text: control.text
            ToolTip.visible: truncated && mouseArea.containsMouse
            ToolTip.delay: 1000
        }

        Item {
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.rightMargin: 5
            Layout.topMargin: 3
            width: 9
            height: 9
            radius: width / 2
            color: control.enabled ? "#009f00" : "#dd0000"
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
