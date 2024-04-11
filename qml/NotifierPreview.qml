import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

MouseArea {
    id: control
    anchors.left: parent.left
    anchors.right: parent.right
    height: childrenRect.height
    cursorShape: control.newNotifier ? Qt.PointingHandCursor : Qt.ArrowCursor
    hoverEnabled: true

    signal customClicked
    property bool rotated: false
    property var notifier
    property bool newNotifier: false

    function handleClick() {
        if (control.rotated) {
            arrowRotation.from = 90
            arrowRotation.to = 0
        } else {
            arrowRotation.from = 0
            arrowRotation.to = 90
        }
        arrowRotation.running = true
        control.customClicked()
        control.rotated = !control.rotated
    }

    onClicked: control.handleClick()

    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 32

        Item {
            Layout.leftMargin: 5
            Layout.preferredHeight: 12
            Layout.preferredWidth: 12

            Image {
                id: arrow
                visible: !control.newNotifier
                source: "qrc:/assets/right-arrow.png"

                RotationAnimator {
                    id: arrowRotation
                    target: arrow
                    duration: 200
                }
            }

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -3
                anchors.horizontalCenter: parent.horizontalCenter
                color: control.containsMouse ? root.accentColor : "#fff"
                width: 20
                height: 20
                radius: width / 2
                visible: control.newNotifier

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: control.containsMouse ? "#fff" : "#005ba1"
                    bottomPadding: 6
                    renderType: Text.NativeRendering
                    font.pixelSize: 26
                    text: '+'
                }
            }
        }

        Text {
            Layout.leftMargin: control.newNotifier ? 5 : 3
            Layout.fillWidth: true
            Layout.bottomMargin: 3
            font.pointSize: 10
            text: notifier.name
            elide: Text.ElideRight
            maximumLineCount: 1
            renderType: Text.NativeRendering
            color: control.newNotifier ? control.containsMouse ? "#004c87" : "#555" : "#000"

            ToolTip.text: notifier.name
            ToolTip.visible: truncated && nameMa.containsMouse
            ToolTip.delay: 1000

            MouseArea {
                id: nameMa
                cursorShape: control.newNotifier ? Qt.PointingHandCursor : Qt.ArrowCursor
                anchors.fill: parent
                hoverEnabled: true
                onClicked: control.handleClick()
            }
        }

        Row {
            visible: !control.newNotifier
            spacing: 8

            CusSwitch {
                checked: notifier.toastEnabled
                backgroundColor_on: root.accentColor

                property bool loaded: false
                Component.onCompleted: loaded = true

                onCheckedChanged: {
                    notifier.toastEnabled = checked
                    if (loaded)
                        root.saveEnabled = true
                }
            }
            CusSwitch {
                checked: notifier.soundEnabled
                backgroundColor_on: root.accentColor

                property bool loaded: false
                Component.onCompleted: loaded = true

                onCheckedChanged: {
                    notifier.soundEnabled = checked
                    if (loaded)
                        root.saveEnabled = true
                }
            }
        }
    }
}
