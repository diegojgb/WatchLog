import QtQuick
import QtQuick.Controls.Fusion

TextField {
    id: control
    renderType: Text.NativeRendering

    property bool error: false

    background: Rectangle {
        color: root.whiteColor
        border.color: control.activeFocus ? "#41adff" : control.error ? "#ff0000" : "#ababab"
        radius: 2
    }
}
