import QtQuick 2.4
import Qt.labs.platform 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11

Window {
    id: window
    title: qsTr("HoloSqape")
    width: 640
    height: 360
    visible: false


    ColumnLayout {
        id: layout
        anchors.fill: parent
    }

    function buildMenu() {
        var apps = Container.installedApps()
        var component = Qt.createComponent("AppWidget.qml");
        for(var i in apps) {
            var hash = apps[i]
            component.createObject(layout,
                {
                    hash: hash,
                    name: Container.appName(hash),
                    container: Container
                }
            )
            console.log(hash)
        }
    }

    Connections {
        target: Container
        onAppsChanged: buildMenu()
    }


    Component.onCompleted: {
        if(Container.installedApps().length === 0) {
            Container.install_app(":/apps/test.json");
        }

        buildMenu()
    }

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
            Container.install_app(dnaDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        visible: false
    }

}
