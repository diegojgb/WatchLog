import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Item {
    id: control
    anchors.left: parent.left
    anchors.right: parent.right
    height: childrenRect.height

    signal customClicked
    property bool rotated: false
    property var notifier
    property bool newNotifier: false
    property Row optionsItem: options
    property bool hovered: controlMa.containsMouse || textField.hovered

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

    MouseArea {
        id: controlMa
        anchors.fill: parent
        cursorShape: control.newNotifier ? Qt.PointingHandCursor : Qt.ArrowCursor
        hoverEnabled: true
        onClicked: control.handleClick()
    }

    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 32

        Item {
            Layout.leftMargin: 5
            Layout.bottomMargin: 2
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
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: -3
                color: control.hovered ? root.accentColor : "#fff"
                width: 20
                height: 20
                radius: width / 2
                visible: control.newNotifier

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: control.hovered ? "#fff" : "#005ba1"
                    bottomPadding: 6
                    renderType: Text.NativeRendering
                    font.pixelSize: 26
                    text: '+'
                }
            }
        }

        TextField {
            id: textField

            property bool focused: false
            property bool custEnabled: false

            text: textField.focused ? notifier.name : metrics.elidedText

            Layout.leftMargin: control.newNotifier ? 1 : -1
            Layout.bottomMargin: control.newNotifier ? 4 : 2
            Layout.fillWidth: true
            color: control.newNotifier ? control.hovered ? "#004c87" : "#555" : "#000"
            renderType: Text.NativeRendering
            font.pointSize: 10
            background.visible: textField.custEnabled
            topPadding: 1
            bottomPadding: 3
            leftPadding: 4
            enabled: textField.custEnabled

            TextMetrics {
                id: metrics
                text: notifier.name
                font: textField.font
                elide: Qt.ElideRight
                elideWidth: textField.width - 5
            }

            ToolTip.text: notifier.name
            ToolTip.visible: hovered && notifier.name !== metrics.elidedText
            ToolTip.delay: 1000

            onActiveFocusChanged: {
                if (activeFocus)
                    textField.focused = true
            }

            onEditingFinished: {
                notifier.name = textField.text
                parent.forceActiveFocus()
                textField.focused = false
                textField.custEnabled = false
            }
        }

        // Edit button
        Row {
            id: options
            Layout.fillHeight: true
            Layout.rightMargin: 10
            spacing: 10
            visible: !textField.focused && options.opacity !== 0

            Rectangle {
                anchors.top: parent.top
                anchors.topMargin: 3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
                width: 25
                color: editButtonMa.containsMouse ? "#ccc" : "#fff"
                radius: 4

                Behavior on color {
                    ColorAnimation {
                        duration: root.transitionDuration
                    }
                }

                Image {
                    id: editButton
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/assets/edit.png"
                }

                MouseArea {
                    id: editButtonMa
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        textField.custEnabled = true
                        textField.forceActiveFocus()
                    }
                }
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: root.transitionDuration
                }
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
