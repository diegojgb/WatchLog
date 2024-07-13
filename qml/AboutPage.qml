import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Pane {
    id: page
    Layout.fillHeight: true
    Layout.fillWidth: true

    background: Rectangle {
        anchors.fill: parent
        color: root.whiteColor
    }

    signal closed

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        height: col.height + 54
        color: "#eee"
        radius: 10

        Image {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 18
            anchors.rightMargin: 20
            source: closeMa.containsMouse ? "qrc:/assets/close-hovered.png" : "qrc:/assets/close.png"

            MouseArea {
                id: closeMa
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: page.closed()
            }
        }

        ColumnLayout {
            id: col

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            spacing: 0

            Row {
                spacing: 10

                Image {
                    source: "qrc:/assets/watchlog-logo-big.png"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Column {
                    Label {
                        text: 'WatchLog'
                        font.pointSize: 18
                        font.weight: Font.DemiBold
                        // renderType: Text.NativeRendering
                    }
                    Label {
                        text: 'Version 1.3.0-dev.2'
                        font.pointSize: 12
                        // renderType: Text.NativeRendering
                    }
                }
            }

            LabelSeparator {
                Layout.topMargin: 24
                text: 'Author info'
            }

            Label {
                Layout.leftMargin: 10
                Layout.topMargin: 10
                text: '© 2024 - Diego Gonzalez'
                font.pointSize: 10
                renderType: Text.NativeRendering
            }

            Label {
                Layout.leftMargin: 26
                text: 'Released: July 2024'
                font.pointSize: 10
                renderType: Text.NativeRendering
            }

            Item {
                width: 1
                height: 5
            }
        }
    }
}
