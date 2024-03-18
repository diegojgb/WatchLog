import QtQuick
import QtQuick.Controls.Material

CheckBox {
    id: control

    Material.accent: "#0078d4"

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
        renderType: Text.NativeRendering
    }
}
