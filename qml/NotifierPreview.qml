import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

MouseArea {
    id: control
    anchors.left: parent.left
    anchors.right: parent.right
    height: childrenRect.height
    hoverEnabled: true

    signal customClicked
    property bool rotated: false
    property var notifier

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

        Image {
            id: arrow
            source: "qrc:/assets/right-arrow.png"
            Layout.leftMargin: 5
            Layout.preferredHeight: 12
            Layout.preferredWidth: 12

            RotationAnimator {
                id: arrowRotation
                target: arrow
                duration: 200
            }
        }

        Text {
            Layout.fillWidth: true
            Layout.bottomMargin: 3
            font.pointSize: 10
            text: notifier.name
            elide: Text.ElideRight
            maximumLineCount: 1
            renderType: Text.NativeRendering

            ToolTip.text: notifier.name
            ToolTip.visible: truncated && mouseArea.containsMouse
            ToolTip.delay: 1000
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: control.handleClick()
            }
        }

        Row {
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
