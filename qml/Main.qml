import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root

    visible: true
    title: qsTr("WatchLog")

    width: 841
    height: 560

    property color accentColor: "#0078d4"
    property color whiteColor: "#fbfbfb"
    property int transitionDuration: 150
    property bool saveEnabled: false
    property int errorCount

    // Hide the window instead of closing it when the close button is clicked
    // @disable-check M16
    onClosing: close => {
                   hide()
                   close.accepted = false
               }

    onErrorCountChanged: {
        if (errorCount > 0)
            root.saveEnabled = false
    }

    Menu {
        id: contextMenu
        closePolicy: Popup.CloseOnPressOutside

        property int tabIndex

        background: Rectangle {
            implicitWidth: 150
            color: "#fff"
            border.color: "#ababab"
            radius: 4
        }

        CustomMenuItem {
            text: "Rename"

            onTriggered: {
                var monitor = stackView.itemAt(contextMenu.tabIndex)
                monitor.renameDialog.open()
            }
        }

        CustomMenuItem {
            text: "Delete"
            textItem.color: "#ff0000"
            textItem.anchors.top: textItem.parent.top
            textItem.anchors.bottom: textItem.parent.bottom
            textItem.anchors.bottomMargin: 3

            onTriggered: {
                var monitor = stackView.itemAt(contextMenu.tabIndex)
                monitor.deleteDialog.open()
            }
        }
    }

    Menu {
        id: notifierMenu
        closePolicy: Popup.CloseOnPressOutside

        property Notifier notifier

        background: Rectangle {
            implicitWidth: 150
            color: "#fff"
            border.color: "#ababab"
            radius: 4
        }

        CustomMenuItem {
            text: "Rename"

            onTriggered: {
                notifierMenu.notifier.rename()
            }
        }

        CustomMenuItem {
            text: "Delete"
            textItem.color: "#ff0000"
            textItem.anchors.top: textItem.parent.top
            textItem.anchors.bottom: textItem.parent.bottom
            textItem.anchors.bottomMargin: 3

            onTriggered: {
                notifierMenu.notifier.openDeleteDialog()
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
                                   contextMenu.tabIndex = idx
                                   contextMenu.popup()
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

                Repeater {
                    model: Manager.monitors

                    MonitorPage {
                        id: monitorPage
                        monitor: model.edit

                        onDeleted: {
                            if (sidebar.tabBar.tabIndex !== 0)
                                sidebar.tabBar.tabIndex = sidebar.tabBar.tabIndex - 1

                            Manager.monitors.remove(model.edit.filePath)
                        }

                        property int prevErrorCount: 0

                        onErrorCountChanged: {
                            var delta = monitorPage.errorCount - monitorPage.prevErrorCount
                            root.errorCount += delta

                            monitorPage.prevErrorCount = monitorPage.errorCount
                        }

                        onRightClicked: notifier => {
                                            notifierMenu.notifier = notifier
                                            notifierMenu.popup()
                                        }
                    }
                }

                HomePage {
                    onClickedAddMonitor: sidebar.openAddMonitorDialog()
                }
            }
        }
    }
}
