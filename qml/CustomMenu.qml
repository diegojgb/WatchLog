import QtQuick
import QtQuick.Controls

Menu {
    id: control
    closePolicy: Popup.CloseOnPressOutside

    property bool dark: false
    property color borderColor: control.dark ? "#5d5e61" : "#ababab"
    property color bgColor: control.dark ? "#3c3d40" : "#fff"

    background: Rectangle {
        implicitWidth: 150
        color: control.bgColor
        border.color: control.borderColor
        radius: 4
    }
}
