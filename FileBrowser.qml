import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Fusion

RowLayout {
    id: control

    required property var filePath
    property color backgroundColor: "#e6e6e6"
    property int fieldHeight: 25

    signal fileAccepted

    FileDialog {
        id: fileDialog
        title: "Select a file"
        selectedFile: "file:///" + control.filePath
        onAccepted: {
            control.filePath = selectedFile.toString().slice(8)
            control.fileAccepted()
        }
    }

    TextField {
        id: pathField
        Layout.fillWidth: true
        Layout.preferredHeight: control.fieldHeight
        enabled: false
        text: control.filePath
        verticalAlignment: Text.AlignVCenter
        leftPadding: 5
        bottomPadding: 4
        placeholderText: "No file selected..."
        renderType: Text.NativeRendering

        background: Rectangle {
            color: control.backgroundColor
            border.color: "#bbb"
        }
    }

    CustomButton {
        Layout.preferredWidth: 80
        Layout.preferredHeight: control.fieldHeight + 1
        text: "Browse..."
        onClicked: fileDialog.open()
    }
}
