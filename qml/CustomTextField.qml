import QtQuick
import QtQuick.Controls.Fusion

TextField {
    id: control
    renderType: Text.NativeRendering
    color: control.readOnly ? "#555" : "#000"

    property bool error: false
    property Rectangle backgroundItem: bg

    Keys.onEscapePressed: {
        control.focus = false
    }

    background: Rectangle {
        id: bg
        color: root.whiteColor
        border.color: control.activeFocus
                      && !control.readOnly ? "#41adff" : control.error ? "#ff0000" : "#ababab"
        radius: 2
    }
}
