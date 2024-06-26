import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

Rectangle {
    id: control
    color: "transparent"

    property var tabBar: tabBarItem

    signal tabRightClicked(int tabIndex)
    signal rightClicked

    function openAddMonitorDialog() {
        addMonitorDialog.open()
    }

    ToolTip {
        id: toolTip
        text: "hello"
        delay: 500
        contentWidth: Math.min(textObj.textWidth + 5, 300)

        contentItem: Text {
            id: textObj
            text: toolTip.text
            wrapMode: Text.WordWrap
            renderType: Text.NativeRendering

            // binding-loop-free width and height:
            readonly property alias textWidth: textMetrics.boundingRect.width
            readonly property alias textHeight: textMetrics.boundingRect.height

            TextMetrics {
                id: textMetrics
                font: textObj.font
                text: textObj.text
                elide: textObj.elide
            }
        }

        background: Rectangle {
            color: "#f0f0f0"
            border.color: "#a0a0a0"
            border.width: 1
            radius: 3
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: mouse => {
                       if (mouse.button === Qt.RightButton) {
                           control.rightClicked()
                       }
                   }
    }

    ColumnLayout {
        anchors.fill: parent

        Row {
            Layout.topMargin: 18
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

        Label {
            Layout.topMargin: 12
            Layout.leftMargin: 12
            text: "Monitors"
            color: '#999'
        }

        ColumnLayout {
            Layout.fillWidth: true

            Flickable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: tabBarItem.height + tabBarItem.anchors.topMargin
                contentHeight: tabBarItem.height + tabBarItem.anchors.topMargin
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar {}
                clip: true

                ColumnLayout {
                    id: tabBarItem
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 8

                    property int tabIndex: 0

                    Repeater {
                        id: tabRepeater
                        model: Manager.monitors

                        property ToolTip toolTipAlias: toolTip

                        Tab {
                            id: tabItem
                            Layout.fillWidth: true
                            Layout.preferredHeight: 30
                            Layout.leftMargin: 10
                            Layout.rightMargin: 10
                            text: model.edit.name
                            enabled: model.edit.enabled
                            selected: tabBarItem.tabIndex === idx
                            onHoveredChanged: {
                                if (tabItem.truncated && tabItem.hovered) {
                                    tabRepeater.toolTipAlias.parent = tabItem
                                    tabRepeater.toolTipAlias.text = model.edit.name
                                    tabRepeater.toolTipAlias.visible = true
                                } else {
                                    tabRepeater.toolTipAlias.visible = false
                                }
                            }

                            property int idx: model.index

                            onClicked: tabBarItem.tabIndex = model.index
                            onRightClicked: tabRightClicked(idx)
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.topMargin: 4
                Layout.rightMargin: 12
                Layout.leftMargin: 12
                height: 1
                color: '#444'
            }

            Rectangle {
                id: addMonitorButton
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                color: mouseArea.containsMouse ? "#4d4d4d" : "#2d2e30"
                radius: 7

                Behavior on color {
                    ColorAnimation {
                        duration: root.transitionDuration
                    }
                }

                Row {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    spacing: 6

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/assets/add-new.png"
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        renderType: Text.NativeRendering
                        font.pointSize: 10
                        color: "white"
                        text: 'Add monitor'
                    }
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onClicked: addMonitorDialog.open()
                }
            }
        }

        Item {
            Layout.preferredWidth: 1
            Layout.fillHeight: true
        }

        SaveButton {
            id: saveButton
            Layout.fillWidth: true
            Layout.topMargin: 20
            Layout.bottomMargin: 25
            Layout.leftMargin: 25
            Layout.rightMargin: 25
        }
    }

    CustomDialog {
        id: addMonitorDialog
        topPadding: 21
        bottomPadding: 24
        width: 330

        property bool alreadyMonitored: false
        property string prevFilePath

        function resetFields() {
            name.text = ""
            fileBrowser.filePath = ""
            addMonitorDialog.alreadyMonitored = false
            fileBrowser.error = false
        }

        ColumnLayout {
            id: dialogColumn
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12

            Label {
                text: "Add monitor"
                renderType: Text.NativeRendering
                font.pointSize: 14
                font.bold: true
            }

            Label {
                Layout.topMargin: 14
                Layout.preferredWidth: parent.width
                text: "Name"
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.DemiBold
            }

            CustomTextField {
                id: name
                Layout.fillWidth: true
                placeholderText: "Enter monitor name..."

                onEditingFinished: name.error = name.text === ""
            }

            Label {
                Layout.topMargin: 10
                Layout.preferredWidth: parent.width
                text: "File path"
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.DemiBold
            }

            FileBrowser {
                id: fileBrowser

                fileDialog.onAccepted: {
                    if (addMonitorDialog.prevFilePath === fileBrowser.filePath)
                        return

                    fileBrowser.error = false
                    addMonitorDialog.alreadyMonitored = false
                }
            }

            // Error: filePath already being monitored.
            Text {
                Layout.topMargin: -2
                Layout.leftMargin: 2
                renderType: Text.NativeRendering
                text: "This file is already being monitored."
                color: "#ff0000"
                font.pointSize: 8
                visible: addMonitorDialog.alreadyMonitored
            }

            Row {
                Layout.topMargin: 30
                Layout.alignment: Qt.AlignRight
                spacing: 5

                CustomButton {
                    text: "Cancel"
                    onClicked: {
                        addMonitorDialog.close()
                        addMonitorDialog.resetFields()
                    }
                }

                CustomButton {
                    colorPreset: CustomButton.Color.Blue
                    text: "Add"

                    onClicked: {
                        if (name.text === "") {
                            name.error = true
                            return
                        }
                        if (fileBrowser.filePath === "") {
                            fileBrowser.error = true
                            return
                        }

                        var success = Manager.addMonitor(name.text,
                                                         fileBrowser.filePath)

                        if (!success) {
                            fileBrowser.error = true
                            addMonitorDialog.alreadyMonitored = true
                            addMonitorDialog.prevFilePath = fileBrowser.filePath
                            return
                        }

                        tabBarItem.tabIndex = tabRepeater.count - 1
                        root.saveEnabled = true
                        addMonitorDialog.close()
                        addMonitorDialog.resetFields()
                    }
                }
            }
        }
    }
}
