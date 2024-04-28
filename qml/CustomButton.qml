import QtQuick
import QtQuick.Controls.Fusion

Button {
    id: control
    font.pointSize: 8

    enum Color {
        Gray,
        Blue,
        Red
    }

    property Text textItem: text
    property Rectangle bgItem: bg
    property int colorPreset: CustomButton.Color.Gray

    readonly property color redBackground: control.pressed ? Qt.rgba(1, 0, 0, 0.15) : root.whiteColor
    readonly property color redBorder: control.hovered ? "#ff6b40" : "#ff0000"
    readonly property color redText: "#ff0000"

    readonly property color blueBackground: control.pressed ? "#d0ffff" : root.whiteColor
    readonly property color blueBorder: control.hovered ? "#44a9ff" : "#0078d4"
    readonly property color blueText: root.accentColor

    readonly property color grayBackground: control.pressed ? "#ddd" : root.whiteColor
    readonly property color grayBorder: control.hovered ? "#666" : "#ababab"
    readonly property color grayText: "#000"


    property color borderColor: {
        switch(control.colorPreset) {
            case CustomButton.Color.Red:
                return control.redBorder
            case CustomButton.Color.Blue:
                return control.blueBorder
            default:
                return control.grayBorder
        }
    }

    property color backgroundColor: {
        switch(control.colorPreset) {
            case CustomButton.Color.Red:
                return control.redBackground
            case CustomButton.Color.Blue:
                return control.blueBackground
            default:
                return control.grayBackground
        }
    }

    property color textColor: {
        switch(control.colorPreset) {
            case CustomButton.Color.Red:
                return control.redText
            case CustomButton.Color.Blue:
                return control.blueText
            default:
                return control.grayText
        }
    }

    contentItem: Text {
        id: text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        bottomPadding: 2
        text: control.text
        font: control.font
        color: control.textColor
        opacity: enabled ? 1.0 : 0.3
        elide: Text.ElideRight
        renderType: Text.NativeRendering
    }

    background: Rectangle {
        id: bg
        anchors.fill: parent
        opacity: enabled ? 1 : 0.3
        color: control.backgroundColor
        border.color: control.borderColor
        border.width: 1
        radius: 2

        Behavior on border.color {
            ColorAnimation {
                duration: 200
            }
        }
    }
}
