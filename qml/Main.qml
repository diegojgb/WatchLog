import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root

    visible: true
    title: qsTr("WatchLog")

    width: 841
    height: 565

    property color accentColor: "#0078d4"
    property color whiteColor: "#fbfbfb"
    property int transitionDuration: 150
    property bool saveEnabled: false
    property int errorCount
    property bool initialized: false
    property Popup toolTipItem: toolTip

    // Hide the window instead of closing it when the close button is clicked
    // @disable-check M16
    onClosing: close => {
                   hide()
                   close.accepted = false
               }

    Component.onCompleted: root.initialized = true

    Popup {
        id: toolTip
        contentWidth: Math.min(textObj.textWidth + 5, parent.width)

        property string text: ""

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

    CustomMenu {
        id: globalMenu

        CustomMenuItem {
            text: "Refresh"
            dark: globalMenu.dark
            first: true
            last: true

            onTriggered: Manager.checkFilesNow()
        }
    }

    CustomMenu {
        id: monitorMenu
        dark: true

        property int tabIndex

        CustomMenuItem {
            text: "Rename"
            first: true
            dark: monitorMenu.dark

            onTriggered: {
                var loader = stackView.itemAt(monitorMenu.tabIndex)
                var monitor = loader.item

                monitor.renameDialog.open()
            }
        }

        CustomMenuItem {
            text: "Delete"
            last: true
            dark: monitorMenu.dark
            textItem.color: "#ff6d62"
            textItem.anchors.top: textItem.parent.top
            textItem.anchors.bottom: textItem.parent.bottom
            textItem.anchors.bottomMargin: 3

            onTriggered: {
                var loader = stackView.itemAt(monitorMenu.tabIndex)
                var monitor = loader.item

                monitor.deleteDialog.open()
            }
        }
    }

    CustomMenu {
        id: notifierMenu

        property Notifier notifier
        property var monitor

        CustomMenuItem {
            text: "Rename"
            first: true

            onTriggered: {
                notifierMenu.notifier.rename()
            }
        }

        CustomMenuItem {
            text: "Delete"
            last: true
            textItem.color: "#ff0000"
            textItem.anchors.top: textItem.parent.top
            textItem.anchors.bottom: textItem.parent.bottom
            textItem.anchors.bottomMargin: 3

            onTriggered: {
                delNDialog.name = notifierMenu.notifier.notifier.name
                delNDialog.idx = notifierMenu.notifier.index
                delNDialog.monitor = notifierMenu.monitor
                delNDialog.open()
            }
        }
    }

    // Makes widgets lose focus when clicked outside.
    Pane {
        anchors.fill: parent
        focusPolicy: Qt.ClickFocus
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Sidebar
        Sidebar {
            id: sidebar
            Layout.fillHeight: true
            Layout.preferredWidth: 190
            color: '#2d2e30'

            onTabRightClicked: idx => {
                                   monitorMenu.tabIndex = idx
                                   monitorMenu.popup()
                               }

            onRightClicked: {
                globalMenu.dark = true
                globalMenu.popup()
            }
        }

        // Separator
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: "#2b2b2b"
        }

        // Mainbar
        Rectangle {
            id: mainbar
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: root.whiteColor

            StackLayout {
                id: stackView
                anchors.fill: parent
                currentIndex: sidebar.tabBar.tabIndex

                onCurrentIndexChanged: {
                    rep.load(currentIndex)
                }

                Repeater {
                    id: rep
                    model: Manager.monitors

                    function load(idx) {
                        if (idx >= rep.count)
                            return

                        if (rep.itemAt(idx).active) {
                            return
                        }

                        rep.itemAt(idx).active = true
                    }

                    Component.onCompleted: load(1)

                    Loader {
                        id: loader
                        active: loader.isFirst
                        asynchronous: !loader.isFirst

                        property bool isFirst: model.index === 0

                        BusyIndicator {
                            id: busyIndicator
                            anchors.centerIn: parent
                            running: !loader.isFirst
                        }

                        onLoaded: {
                            busyIndicator.running = false

                            if (!loader.isFirst)
                                rep.load(model.index + 1)
                        }

                        sourceComponent: MonitorPage {
                            id: monitorPage
                            monitor: model.edit

                            onDeleted: {
                                if (sidebar.tabBar.tabIndex !== 0)
                                    sidebar.tabBar.tabIndex = sidebar.tabBar.tabIndex - 1

                                Manager.monitors.remove(model.edit.filePath)
                            }

                            property int prevErrorCount: 0

                            onFileErrorChanged: monitorPage.errorCount += fileError ? 1 : -1

                            onErrorCountChanged: {
                                var delta = monitorPage.errorCount - monitorPage.prevErrorCount
                                root.errorCount += delta

                                monitorPage.prevErrorCount = monitorPage.errorCount
                            }

                            onDeletedNotifier: notifier => {
                                                   delNDialog.name = notifier.notifier.name
                                                   delNDialog.idx = notifier.index
                                                   delNDialog.monitor = model.edit
                                                   delNDialog.open()
                                               }

                            onNotifierRightClicked: notifier => {
                                                        notifierMenu.notifier = notifier
                                                        notifierMenu.monitor = model.edit
                                                        notifierMenu.popup()
                                                    }

                            onRightClicked: {
                                globalMenu.dark = false
                                globalMenu.popup()
                            }
                        }
                    }
                }

                HomePage {
                    onClickedAddMonitor: sidebar.openAddMonitorDialog()
                }
            }
        }
    }

    CustomDialog {
        id: delNDialog

        property var monitor
        property string name
        property int idx

        ColumnLayout {
            id: dialogColumn
            anchors.top: parent.top
            anchors.topMargin: 14
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 12

            Label {
                text: "Delete?"
                renderType: Text.NativeRendering
                font.pointSize: 12
                font.bold: true
            }

            TextMetrics {
                id: metrics
                text: delNDialog.name
                elide: Qt.ElideRight
                elideWidth: 300
            }

            Label {
                Layout.topMargin: 10
                Layout.preferredWidth: parent.width
                text: "Are you sure you want to delete <b>\"" + metrics.elidedText + "\"</b>?"
                wrapMode: Text.Wrap
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
            }

            Row {
                Layout.preferredHeight: 30
                Layout.topMargin: 25
                Layout.alignment: Qt.AlignRight
                Layout.bottomMargin: 20
                spacing: 5

                CustomButton {
                    text: "Cancel"
                    onClicked: delNDialog.close()
                }

                CustomButton {
                    colorPreset: CustomButton.Color.Red
                    text: "Delete"

                    onClicked: {
                        delNDialog.monitor.removeNotifier(delNDialog.idx)
                        root.saveEnabled = true
                        delNDialog.close()
                    }
                }
            }
        }
    }
}
