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
    signal addedNew

    ColumnLayout {
        id: mainColumn
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
                id: regexField

                property string initValue

                Layout.fillWidth: true
                leftPadding: 5
                text: notifier.regexStr
                placeholderText: "Enter a regex expression"

                onActiveFocusChanged: {
                    if (activeFocus)
                        initValue = text
                }

                onEditingFinished: {
                    if (isValid() && initValue !== text) {
                        notifier.regexStr = text
                        root.saveEnabled = true
                    }
                }

                function isValid() {
                    if (regexField.text === "") {
                        regexField.error = true
                        return false
                    }

                    try {
                        let regexp = new RegExp(regexField.text)
                    } catch (error) {
                        regexField.error = true
                        return false
                    }

                    regexField.error = false
                    return true
                }
            }
        }

        Text {
            Layout.leftMargin: descLabel.width + firstItem.spacing
            Layout.topMargin: -2
            renderType: Text.NativeRendering
            text: "Invalid regex expression."
            color: "#ff0000"
            font.pointSize: 8
            visible: regexField.error
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
                    if (initValue !== text) {
                        notifier.title = text
                        root.saveEnabled = true
                    }
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
                    if (initValue !== text) {
                        notifier.desc = text
                        root.saveEnabled = true
                    }
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
                    if (loaded) {
                        notifier.sticky = checked
                        root.saveEnabled = true
                    }
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
                onClicked: control.canceled()
            }

            CustomButton {
                width: 80
                height: 24
                colorPreset: CustomButton.Color.Blue
                text: "Add"

                onClicked: {
                    if (regexField.isValid())
                        control.addedNew()
                }
            }
        }

        Item {
            width: 1
            height: control.custBottomMargin - mainColumn.spacing
        }
    }
}
