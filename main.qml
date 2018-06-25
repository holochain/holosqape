import QtQuick 2.4
import Qt.labs.platform 1.0
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.2

Window {
    id: root
    visible: false





    SystemTrayIcon {
        visible: true
        iconSource: "qrc:/images/Holochain50+alpha.png"

        menu: Menu {
            MenuItem {
                text: qsTr("Install new app...")
                onTriggered: {
                    dnaDialog.open()
                }
            }
            MenuItem {
                text: qsTr("Install new UI component...")
            }
            MenuSeparator{}
            Menu {
                title: qsTr("Apps")
                id: appsMenu

                function buildMenu() {
                    var apps = Container.installedApps()
                    for(var i in apps) {
                        var hash = apps[i]
                        appsMenu.addItem(hash)
                        console.log(hash)
                    }
                }
            }

            MenuSeparator{}
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }

    }

    MenuItem {
        id: wurst
        text: 'wurst'
    }


    FileDialog {
        id: dnaDialog
        title: "Please choose a Holochain DNA file"
        //folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + dnaDialog.fileUrls)
            Container.install_app(dnaDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        visible: false
    }

    Connections {
        target: Container
        onAppsChanged: appsMenu.buildMenu()
    }


    Component.onCompleted: appsMenu.buildMenu()

}
