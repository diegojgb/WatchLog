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
                Layout.fillWidth: true
                leftPadding: 5
                text: notifier.regexStr
                renderType: Text.NativeRendering

                onEditingFinished: {
                    notifier.regexStr = text
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

                onEditingFinished: {
                    notifier.title = text
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

                onEditingFinished: {
                    notifier.desc = text
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

                onFileAccepted: {
                    notifier.imagePath = filePath
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

                onFileAccepted: {
                    notifier.soundPath = filePath
                }
            }
        }

        MaterialCheckBox {
            id: lastItem
            Layout.bottomMargin: control.custBottomMargin
            text: "Sticky notification"
            checked: notifier.sticky
            Layout.leftMargin: descLabel.width - 3
            onCheckedChanged: {
                notifier.sticky = checked
            }
        }
    }
}
