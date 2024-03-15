import QtQuick
import QtQuick.Layouts
import TaoQuick 1.0

RowLayout {
    width: parent.width

    property var notifier

    Text {
        Layout.leftMargin: 5
        font.pointSize: 10
        renderType: Text.NativeRendering
        text: notifier.name
    }

    Item {
        Layout.fillWidth: true
    }

    Row {
        spacing: 8

        CusSwitch {
            checked: notifier.toastEnabled
            backgroundColor_on: root.accentColor

            onCheckedChanged: {
                notifier.toastEnabled = checked
            }
        }
        CusSwitch {
            checked: notifier.soundEnabled
            backgroundColor_on: root.accentColor

            onCheckedChanged: {
                notifier.soundEnabled = checked
            }
        }
    }
}
