import QtQuick
import QtQuick.Controls.Fusion

TextField {
    id: control
    renderType: Text.NativeRendering

    background: Rectangle {
        border.color: control.activeFocus ? "#41adff" : "#ababab"
        radius: 2
    }
}
