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
    required property bool notifierError

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
                property bool errorException: false

                Layout.fillWidth: true
                leftPadding: 5
                placeholderText: "Enter a regex expression"
                text: notifier.regexStr
                error: !regexField.errorException && notifier.regexError

                onActiveFocusChanged: {
                    if (activeFocus)
                        initValue = text
                }

                onEditingFinished: {
                    if (initValue !== text) {
                        notifier.regexStr = text

                        if (!control.newNotifier)
                            root.saveEnabled = true
                    }

                    regexField.errorException = false
                }

                Component.onCompleted: {
                    if (control.newNotifier)
                        regexField.errorException = true
                }
            }
        }

        Text {
            Layout.leftMargin: descLabel.width + firstItem.spacing + 1
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

                        if (!control.newNotifier)
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

                        if (!control.newNotifier)
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
                id: imageBrowser
                backgroundColor: root.whiteColor
                fieldHeight: 23
                filePath: notifier.imagePath
                fileDialog.nameFilters: ["Image files (*.jpg *.jpeg *.png)"]
                error: notifier.imageFileError

                property bool loaded: false
                Component.onCompleted: loaded = true

                onFileAccepted: notifier.imagePath = filePath
                onSelectedFileChanged: {
                    if (loaded && !control.newNotifier)
                        root.saveEnabled = true
                }
            }
        }

        Text {
            Layout.leftMargin: descLabel.width + firstItem.spacing + 1
            Layout.topMargin: -2
            renderType: Text.NativeRendering
            text: "File not available."
            color: "#ff0000"
            font.pointSize: 8
            visible: imageBrowser.error
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
                id: soundBrowser
                backgroundColor: root.whiteColor
                fieldHeight: 23
                filePath: notifier.soundPath
                fileDialog.nameFilters: ["WAV files (*.wav)"]
                error: notifier.soundFileError

                property bool loaded: false
                Component.onCompleted: loaded = true

                onFileAccepted: notifier.soundPath = filePath
                onSelectedFileChanged: {
                    if (loaded && !control.newNotifier)
                        root.saveEnabled = true
                }
            }
        }

        Text {
            Layout.leftMargin: descLabel.width + firstItem.spacing + 1
            Layout.topMargin: -2
            renderType: Text.NativeRendering
            text: "File not available."
            color: "#ff0000"
            font.pointSize: 8
            visible: soundBrowser.error
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
                    if (loaded && !control.newNotifier)
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

                        if (!control.newNotifier)
                            root.saveEnabled = true
                    }
                }
            }
        }

        Loader {
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
            Layout.topMargin: -12
            Layout.bottomMargin: 6
            sourceComponent: control.newNotifier ? addButtons : undefined
        }

        Component {
            id: addButtons

            Row {
                spacing: 5

                CustomButton {
                    text: "Cancel"
                    onClicked: {
                        regexField.errorException = true
                        control.canceled()
                    }
                }

                CustomButton {
                    colorPreset: CustomButton.Color.Blue
                    text: "Add"

                    onClicked: {
                        regexField.errorException = false

                        if (!control.notifierError)
                            control.addedNew()
                    }
                }
            }
        }

        Item {
            width: 1
            height: control.custBottomMargin - mainColumn.spacing
        }
    }
}
