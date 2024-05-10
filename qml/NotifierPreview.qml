import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Item {
    id: control
    anchors.left: parent.left
    anchors.right: parent.right
    height: childrenRect.height

    property var notifier
    property bool newNotifier: false
    property bool newNotifierOngoing: false
    property Row optionsItem: options
    property Image exclamationItem: exclamation
    property bool textFieldError: textField.error
    property bool optionsAux: true
    required property bool notifierError

    signal rightClicked
    signal customClicked
    signal deleted

    function rename() {
        textField.custFocus()
    }

    function handleClick() {
        if (!newNotifierOngoing)
            control.customClicked()

        if (arrow.visible)
            arrow.rotate()

        if (control.newNotifier) {
            control.newNotifier = false
            control.newNotifierOngoing = true
            textField.text = ""
            textField.custFocus()
        }
    }

    function cancelNew() {
        control.newNotifier = true
        control.newNotifierOngoing = false
        textField.text = "Add notification"
        textField.custUnfocus()
    }

    function finishNew() {
        control.newNotifierOngoing = false
        textField.custUnfocus()
        arrow.rotated = true
        arrow.rotate()
    }

    MouseArea {
        id: controlMa
        anchors.fill: parent
        cursorShape: control.newNotifier ? Qt.PointingHandCursor : Qt.ArrowCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        focusPolicy: Qt.ClickFocus
        z: newNotifier ? 1 : 0

        onClicked: mouse => {
                       if (mouse.button === Qt.LeftButton) {
                           control.handleClick()
                       } else if (mouse.button === Qt.RightButton) {
                           control.rightClicked()
                       }
                   }
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
            visible: !newNotifierOngoing

            Image {
                id: arrow
                visible: !control.newNotifier && !control.newNotifierOngoing
                source: "qrc:/assets/right-arrow.png"

                property bool rotated: false

                function rotate() {
                    if (arrow.rotated) {
                        arrowRotation.from = 90
                        arrowRotation.to = 0
                    } else {
                        arrowRotation.from = 0
                        arrowRotation.to = 90
                    }

                    arrowRotation.running = true
                    arrow.rotated = !arrow.rotated
                }

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
                color: controlMa.containsMouse ? root.accentColor : root.whiteColor
                width: 20
                height: 20
                radius: width / 2
                visible: control.newNotifier || control.newNotifierOngoing

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: controlMa.containsMouse ? "#fff" : "#005ba1"
                    bottomPadding: 6
                    renderType: Text.NativeRendering
                    font.pixelSize: 26
                    text: '+'
                }
            }
        }

        CustomTextField {
            id: textField

            property bool focused: false
            property bool custEnabled: false

            text: textField.focused ? notifier.name : metrics.elidedText
            placeholderText: "Enter notification name"

            Layout.leftMargin: control.newNotifierOngoing ? 0 : control.newNotifier ? 1 : -1
            Layout.bottomMargin: control.newNotifier ? 4 : 2
            Layout.fillWidth: true
            Layout.preferredHeight: 24
            color: control.newNotifier ? controlMa.containsMouse ? "#004c87" : "#555" : "#000"
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

            // Enter or loses focus
            onEditingFinished: {
                if (textField.text === "") {
                    if (control.newNotifierOngoing)
                        textField.error = true
                    else
                        textField.text = notifier.name
                } else {
                    if (textField.text !== notifier.name) {
                        notifier.name = textField.text

                        if (!control.newNotifierOngoing)
                            root.saveEnabled = true
                    }

                    textField.error = false
                }

                if (!control.newNotifierOngoing) {
                    textField.custEnabled = false
                }

                textField.focused = false
            }

            // Enter only
            onAccepted: parent.forceActiveFocus()

            function custFocus() {
                textField.custEnabled = true
                textField.forceActiveFocus()
            }

            function custUnfocus() {
                textField.focused = false
                textField.custEnabled = false
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: childrenRect.width
            Layout.rightMargin: 3

            Image {
                id: exclamation
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                visible: control.notifierError && !control.newNotifier
                         && !control.newNotifierOngoing
                         && exclamation.opacity !== 0

                source: "qrc:/assets/exclamation.png"

                Behavior on opacity {
                    NumberAnimation {
                        duration: root.transitionDuration
                    }
                }
            }

            // Edit and trash buttons
            Row {
                id: options
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                spacing: 5
                visible: !control.newNotifier && !control.newNotifierOngoing
                         && !textField.focused && options.opacity !== 0
                         && control.optionsAux

                // Edit Button
                Rectangle {
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 3
                    width: 26
                    color: editButtonMa.containsMouse ? "#ccc" : root.whiteColor
                    radius: 4

                    Behavior on color {
                        ColorAnimation {
                            duration: root.transitionDuration
                        }
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "qrc:/assets/edit.png"
                    }

                    MouseArea {
                        id: editButtonMa
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        z: 2

                        onClicked: {
                            textField.custFocus()
                        }
                    }
                }

                // Trash Button
                Rectangle {
                    anchors.top: parent.top
                    anchors.topMargin: 1
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 2.5
                    width: 26
                    color: trashButtonMa.containsMouse ? "#ccc" : root.whiteColor
                    radius: 4

                    Behavior on color {
                        ColorAnimation {
                            duration: root.transitionDuration
                        }
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "qrc:/assets/trash.png"
                    }

                    MouseArea {
                        id: trashButtonMa
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: control.deleted()
                    }
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: root.transitionDuration
                    }
                }
            }
        }

        Row {
            visible: !control.newNotifier
            spacing: 8

            CusSwitch {
                checked: notifier.toastEnabled
                enabled: !control.notifierError && !control.newNotifierOngoing
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
                enabled: !control.notifierError && !control.newNotifierOngoing
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
