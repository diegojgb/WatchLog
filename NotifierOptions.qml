import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Item {
    id: control
    height: childrenRect.height

    required property var notifier
    property int cusTopMargin
    property int custBottomMargin

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
            TextField {
                id: field

                property string initValue

                Layout.fillWidth: true
                leftPadding: 5
                text: notifier.regexStr
                renderType: Text.NativeRendering

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
            TextField {
                Layout.fillWidth: true
                leftPadding: 5
                text: notifier.title
                renderType: Text.NativeRendering

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
            TextField {
                Layout.fillWidth: true
                leftPadding: 5
                text: notifier.desc
                renderType: Text.NativeRendering

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
                backgroundColor: "#fff"
                fieldHeight: 23
                filePath: notifier.imagePath

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
                backgroundColor: "#fff"
                fieldHeight: 23
                filePath: notifier.soundPath

                property bool loaded: false
                Component.onCompleted: loaded = true

                onFileAccepted: notifier.soundPath = filePath
                onSelectedFileChanged: {
                    if (loaded)
                        root.saveEnabled = true
                }
            }
        }

        MaterialCheckBox {
            id: lastItem
            Layout.bottomMargin: control.custBottomMargin
            text: "Sticky notification"
            checked: notifier.sticky
            Layout.leftMargin: descLabel.width - 3

            property bool loaded: false
            Component.onCompleted: loaded = true

            onCheckedChanged: {
                notifier.sticky = checked
                if (loaded)
                    root.saveEnabled = true
            }
        }
    }
}
