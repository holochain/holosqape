import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4

ApplicationWindow {
    id: window
    title: qsTr("HoloSqape")
    width: 500
    height: 680
    visible: true

    property var activeRootUI

    Rectangle {
        id: mainView
        anchors.top: parent.top
        anchors.left: dock.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: 'blue'
    }



    Dock {
        id: dock
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        onConsoleIconClicked: jsConsole.visible = !jsConsole.visible
        onRootUISelected: function(rootUIName){
            console.log(rootUIName)
            var component = Qt.createComponent("file://"+Container.rootUIsDirectoryPath()+"/"+rootUIName+"/main.qml");
            while(component.status !== Component.Ready && component.status !== Component.Error){
                console.log(component.status)
            }
            console.log(component.errorString())

            activeRootUI = component.createObject(mainView, {
              "anchors.fill": mainView
            })
        }
    }



    Console {
        id: jsConsole
        visible: false
        anchors.left: dock.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }



    Component.onCompleted: {
        if(Container.installedApps().length === 0) {
            Container.installApp(":/apps/test.json");
            Container.installApp(":/apps/info.json");
        }
    }

    Loader {
        source: (Qt.platform.os == "linux" || Qt.platform.os == "osx" ) ? "SystemTrayMenu.qml" : ""
    }






}
