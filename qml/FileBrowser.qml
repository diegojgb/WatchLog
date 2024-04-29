import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Fusion

RowLayout {
    id: control

    property string filePath: ""
    property color backgroundColor: "#f5f5f5"
    property int fieldHeight: 25
    property FileDialog fileDialog: fileDialogItem

    signal fileAccepted
    signal selectedFileChanged(string newFilePath)

    FileDialog {
        id: fileDialogItem
        title: "Select a file"
        selectedFile: "file:///" + control.filePath
        fileMode: FileDialog.OpenFile
        currentFolder: "file:///" + control.filePath.substring(
                           0, control.filePath.lastIndexOf("/"))

        onAccepted: {
            control.filePath = selectedFile.toString().slice(8)
            control.fileAccepted()
        }
        onSelectedFileChanged: control.selectedFileChanged(selectedFile)
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
            border.color: "#ababab"
            radius: 2
        }
    }

    CustomButton {
        Layout.preferredHeight: control.fieldHeight
        text: "Browse..."
        onClicked: fileDialog.open()
    }
}
