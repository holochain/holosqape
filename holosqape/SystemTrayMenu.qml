import QtQuick 2.0
import Qt.labs.platform 1.0
import QtQuick.Dialogs 1.0

Item {
    SystemTrayIcon {
        visible: true
        iconSource: "qrc:/images/HoloSqape-system-tray.png"

        menu: Menu {
            MenuItem {
                text: qsTr("Install new app...")
                onTriggered: {
                    dnaDialog.open()
                }
            }
            MenuItem {
                text: qsTr("Install new UI component...")
                onTriggered: {
                    uiDialog.open()
                }
            }
            MenuSeparator{}
            MenuItem {
                text: window.visible ? qsTr("Hide main window") : qsTr("Show main window")
                onTriggered: { window.visible = !window.visible }
            }

            MenuSeparator{}
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }

    }


    FileDialog {
        id: dnaDialog
        title: "Please choose a Holochain DNA file"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + dnaDialog.fileUrls)
            Container.installApp(dnaDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        visible: false
    }

    FileDialog {
        id: uiDialog
        title: "Please choose a QML component directory"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + uiDialog.fileUrls)
            Container.installRootUI(uiDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        visible: false
        selectFolder: true
    }
}
