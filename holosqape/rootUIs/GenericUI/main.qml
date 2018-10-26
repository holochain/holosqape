import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4

Rectangle {
    ScrollView {
        horizontalScrollBarPolicy: 0
        anchors.fill: parent
        anchors.margins: 20

        Flickable {
            ColumnLayout {
                id: layout
                anchors.fill: parent
                Layout.bottomMargin: 150
            }
        }
    }

    Connections {
        target: Container
        onAppsChanged: buildMenu()
    }

    Component.onCompleted: {
        buildMenu()
    }

    function buildMenu() {
        var apps = Container.installedApps()
        var component = Qt.createComponent("AppWidget.qml");
        for(var i in apps) {
            var hash = apps[i]
            var instance = Container.instantiate(hash)
            instance.start()
            component.createObject(layout,
                {
                    hash: hash,
                    name: Container.appName(hash),
                    app: instance
                }
            )
            console.log(hash)
        }
    }

}
