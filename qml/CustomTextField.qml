import QtQuick
import QtQuick.Controls.Fusion

TextField {
    id: control
    renderType: Text.NativeRendering
    color: control.readOnly ? "#555" : "#000"

    property bool error: false
    property Rectangle backgroundItem: bg

    background: Rectangle {
        id: bg
        color: root.whiteColor
        border.color: control.readOnly ? "#ababab" : control.activeFocus ? "#41adff" : control.error ? "#ff0000" : "#ababab"
        radius: 2
    }
}
