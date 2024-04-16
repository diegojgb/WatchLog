import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Item {
    id: control
    height: childrenRect.height

    required property var notifier
    property int cusTopMargin
    property int custBottomMargin
    property bool newNotifier: false

    signal canceled

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right

        RowLayout {
            id: firstItem
            Layout.topMargin: control.cusTopMargin
            Layout.fillWidth: true
            spacing: 5

            Label {
                text: "Regex:"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: descLabel.width
                renderType: Text.NativeRendering
            }

            CustomTextField {
                id: field

                property string initValue

                Layout.fillWidth: true
                leftPadding: 5
                text: notifier.regexStr

                onActiveFocusChanged: {
                    if (activeFocus)
                        initValue = text
                }

                onEditingFinished: {
                    notifier.regexStr = text
                    if (initValue !== text)
                        root.saveEnabled = true
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 5

            Label {
                text: "Title:"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: descLabel.width
                renderType: Text.NativeRendering
            }

            CustomTextField {
                Layout.fillWidth: true
                leftPadding: 5
                text: notifier.title

                property string initValue

                onActiveFocusChanged: {
                    if (activeFocus)
                        initValue = text
                }

                onEditingFinished: {
                    notifier.title = text
                    if (initValue !== text)
                        root.saveEnabled = true
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 5

            Label {
                id: descLabel
                text: "Description:"
                renderType: Text.NativeRendering
            }

            CustomTextField {
                Layout.fillWidth: true
                leftPadding: 5
                text: notifier.desc

                property string initValue

                onActiveFocusChanged: {
                    if (activeFocus)
                        initValue = text
                }

                onEditingFinished: {
                    notifier.desc = text
                    if (initValue !== text)
                        root.saveEnabled = true
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 5

            Label {
                text: "Image:"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: descLabel.width
                renderType: Text.NativeRendering
            }
            FileBrowser {
                backgroundColor: root.whiteColor
                fieldHeight: 23
                filePath: notifier.imagePath
                fileDialog.nameFilters: ["Image files (*.jpg *.jpeg *.png)"]

                property bool loaded: false
                Component.onCompleted: loaded = true

                onFileAccepted: notifier.imagePath = filePath
                onSelectedFileChanged: {
                    if (loaded)
                        root.saveEnabled = true
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 5

            Label {
                text: "Sound:"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: descLabel.width
                renderType: Text.NativeRendering
            }
            FileBrowser {
                backgroundColor: root.whiteColor
                fieldHeight: 23
                filePath: notifier.soundPath
                fileDialog.nameFilters: ["WAV files (*.wav)"]

                property bool loaded: false
                Component.onCompleted: loaded = true

                onFileAccepted: notifier.soundPath = filePath
                onSelectedFileChanged: {
                    if (loaded)
                        root.saveEnabled = true
                }
            }
        }

        Row {
            id: lastItem
            spacing: 5

            Label {
                width: descLabel.width
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                text: "Duration:"
                renderType: Text.NativeRendering
            }

            CustomComboBox {
                anchors.verticalCenter: parent.verticalCenter
                enabled: !checkbox.checked

                property var stringToIndex: {
                    "System": 0,
                    "Short": 1,
                    "Long": 2
                }

                property bool loaded: false
                Component.onCompleted: loaded = true

                currentIndex: stringToIndex[notifier.duration]
                model: ["System", "Short", "Long"]

                onCurrentIndexChanged: {
                    notifier.duration = model[currentIndex]
                    if (loaded)
                        root.saveEnabled = true
                }
            }

            MaterialCheckBox {
                id: checkbox
                text: "Sticky notification"
                checked: notifier.sticky

                property bool loaded: false
                Component.onCompleted: loaded = true

                onCheckedChanged: {
                    notifier.sticky = checked
                    if (loaded)
                        root.saveEnabled = true
                }
            }
        }

        Row {
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
            Layout.topMargin: -12
            Layout.bottomMargin: 6
            visible: control.newNotifier
            spacing: 5

            CustomButton {
                width: 80
                height: 24
                text: "Cancel"
                onClicked: {
                    control.canceled()
                }
            }
            CustomButton {
                width: 80
                height: 24
                bgItem.color: pressed ? "#d0ffff" : root.whiteColor
                bgItem.border.color: hovered ? "#44a9ff" : "#0078d4"
                textItem.color: "#0078d4"
                text: "Add"
            }
        }

        // CustomButton {
        //     Layout.alignment: Qt.AlignRight | Qt.AlignTop
        //     Layout.topMargin: -12
        //     Layout.bottomMargin: 6
        //     Layout.preferredWidth: 80
        //     Layout.preferredHeight: 24
        //     visible: !control.newNotifier

        //     bgItem.color: pressed ? Qt.rgba(1, 0, 0, 0.15) : root.whiteColor
        //     bgItem.border.color: hovered ? "#ff6b40" : "#ff0000"
        //     textItem.color: "#ff0000"
        //     text: "Delete"

        //     onClicked: deleteDialog.open()
        // }
        Item {
            width: 1
            height: control.custBottomMargin - 5 // 5 is the default spacing of the ColumnLayout.
        }
    }
}
