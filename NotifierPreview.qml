import QtQuick
import QtQuick.Layouts
import TaoQuick 1.0

MouseArea {
    id: control
    anchors.left: parent.left
    anchors.right: parent.right
    height: childrenRect.height

    signal customClicked
    property bool rotated: false
    property var notifier

    onClicked: {
        if (rotated) {
            arrowRotation.from = 90
            arrowRotation.to = 0
        } else {
            arrowRotation.from = 0
            arrowRotation.to = 90
        }
        arrowRotation.running = true
        control.customClicked()
        rotated = !rotated
    }

    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right

        Image {
            id: arrow
            source: "assets/right-arrow.png"
            Layout.leftMargin: 5
            Layout.preferredHeight: 12
            Layout.preferredWidth: 12

            RotationAnimator {
                id: arrowRotation
                target: arrow
                duration: 1000
            }
        }

        Text {
            Layout.bottomMargin: 3
            font.pointSize: 10
            renderType: Text.NativeRendering
            text: notifier.name
        }

        Item {
            Layout.fillWidth: true
        }

        Row {
            spacing: 8

            CusSwitch {
                checked: notifier.toastEnabled
                backgroundColor_on: root.accentColor

                onCheckedChanged: {
                    notifier.toastEnabled = checked
                }
            }
            CusSwitch {
                checked: notifier.soundEnabled
                backgroundColor_on: root.accentColor

                onCheckedChanged: {
                    notifier.soundEnabled = checked
                }
            }
        }
    }
}
