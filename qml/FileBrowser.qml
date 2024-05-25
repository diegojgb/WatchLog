import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Fusion

RowLayout {
    id: control

    property string filePath: ""
    property color backgroundColor: "#f5f5f5"
    property int fieldHeight: 24
    property FileDialog fileDialog: fileDialogItem
    property bool error: false

    signal fileAccepted
    signal selectedFileChanged(string newFilePath)

    FileDialog {
        id: fileDialogItem
        title: "Select a file"
        selectedFile: "file:///" + control.filePath
        fileMode: FileDialog.OpenFile

        onAccepted: {
            control.filePath = selectedFile.toString().slice(8)
            control.fileAccepted()
            fileDialogItem.currentFolder = ""
        }
        onSelectedFileChanged: control.selectedFileChanged(selectedFile)

        Component.onCompleted: {
            if (filePath !== "")
                fileDialogItem.currentFolder = "file:///" + control.filePath.substring(
                            0, control.filePath.lastIndexOf("/"))
        }
    }

    CustomTextField {
        id: pathField
        Layout.fillWidth: true
        Layout.preferredHeight: control.fieldHeight
        readOnly: true
        text: control.filePath
        verticalAlignment: Text.AlignVCenter
        leftPadding: 5
        bottomPadding: 4
        placeholderText: "No file selected..."
        renderType: Text.NativeRendering
        backgroundItem.color: control.backgroundColor
        error: control.error
    }

    CustomButton {
        Layout.preferredHeight: control.fieldHeight
        text: "Browse..."
        onClicked: fileDialog.open()
    }
}
