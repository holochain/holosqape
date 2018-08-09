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


    ScrollView {
        horizontalScrollBarPolicy: 0
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: consoleOutput.top
        anchors.margins: 20

        Flickable {
            ColumnLayout {
                id: layout
                anchors.fill: parent
                Layout.bottomMargin: 150
            }
        }
    }



    TextArea {
        id: consoleOutput
        height: 100
        antialiasing: true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: consoleInput.top
        readOnly: true
    }



    Rectangle {
        id: consoleInput
        height: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        TextInput {
            anchors.topMargin: 2
            anchors.leftMargin: 5
            cursorVisible: false
            antialiasing: true
            anchors.fill: parent


            property var history: []
            property int historyIndex: 0

            onAccepted: {
                try {
                    var result = eval(this.text)
                    consoleOutput.append("> "+text+"\n"+result)
                } catch(e) {
                    consoleOutput.append("> "+text+"\n"+e)
                }
                history.unshift(this.text)
                this.text = ""
                historyIndex = 0
            }

            Keys.onPressed: {
                if(event.key === Qt.Key_Up) {
                    if(historyIndex < history.length){
                        this.text = history[historyIndex]
                        historyIndex++
                    }
                }

                if(event.key === Qt.Key_Down) {
                    historyIndex--
                    if(historyIndex < 0) historyIndex = 0
                    if(historyIndex < history.length){
                        this.text = history[historyIndex]
                    } else {
                        this.text = ""
                    }
                }
            }
        }
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
                    app: Container.instantiate(hash)
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
            Container.installApp(":/apps/test.json");
            Container.installApp(":/apps/info.json");
        }

        buildMenu()
    }

    Loader {
        source: (Qt.platform.os == "linux" || Qt.platform.os == "osx" ) ? "SystemTrayMenu.qml" : ""
    }






}
