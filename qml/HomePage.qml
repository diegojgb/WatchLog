import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    Layout.fillHeight: true
    Layout.fillWidth: true

    signal clickedAddMonitor

    background: Rectangle {
        anchors.fill: parent
        color: root.whiteColor
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        spacing: 20

        Image {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 200
            Layout.preferredHeight: 200
            source: "qrc:/assets/home-image.png"
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "You don't have any monitors yet"
            font.bold: true
            font.pixelSize: 18
        }

        Text {
            text: "Start monitoring your first file by creating a <b>monitor</b> and adding some <b>notifications.</b>"
            Layout.preferredWidth: 350
            Layout.alignment: Qt.AlignHCenter
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 14
        }

        FilledButton {
            Layout.alignment: Qt.AlignHCenter
            text: "Add monitor"
            textIconItem.visible: true
            textItem.font.bold: true

            onClicked: clickedAddMonitor()
        }
    }
}
