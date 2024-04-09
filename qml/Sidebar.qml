import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: control

    property var tabBar: tabBarItem

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right

        Row {
            Layout.topMargin: 15
            Layout.leftMargin: 14
            spacing: 8

            Image {
                id: logo
                source: "qrc:/assets/watchlog-logo.png"
            }
            Label {
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                text: "WatchLog"
                color: "#fff"
                font.pointSize: 13
                font.weight: Font.DemiBold
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 8
            Layout.rightMargin: 12
            Layout.leftMargin: 12
            height: 1
            color: '#444'
        }

        Label {
            Layout.leftMargin: 12
            text: "Monitors"
            color: '#888'
        }

        ColumnLayout {
            id: tabBarItem
            Layout.fillWidth: true
            Layout.topMargin: 10

            property int tabIndex: 0

            Repeater {
                id: tabRepeater

                model: Manager.monitorsOrder

                Tab {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    text: Manager.hashGet(modelData).name
                    enabled: Manager.hashGet(modelData).enabled
                    selected: tabBarItem.tabIndex == index

                    property int index: tabRepeater.model.indexOf(modelData)

                    onClicked: {
                        tabBarItem.tabIndex = tabRepeater.model.indexOf(
                                    modelData)
                    }
                }
            }
        }
    }
    // Save button
    SaveButton {}
}
