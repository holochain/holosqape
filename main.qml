import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4

ApplicationWindow {
    id: window
    title: qsTr("HoloSqape")
    width: 640
    height: 360
    visible: true


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
            Container.install_app(":/apps/info.json");
        }

        buildMenu()
    }

    Loader {
        source: (Qt.platform.os == "linux" || Qt.platform.os == "osx" ) ? "SystemTrayMenu.qml" : ""
    }






}
