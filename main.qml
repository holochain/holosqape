import QtQuick 2.4
import Qt.labs.platform 1.0

SystemTrayIcon {
    visible: true
    iconSource: "qrc:/images/Holochain50+alpha.png"

    menu: Menu {
        MenuItem {
            text: qsTr("Install new app...")
        }
        MenuItem {
            text: qsTr("Install new UI component...")
        }
        MenuSeparator{}
        MenuItem {
            text: qsTr("Quit")
            onTriggered: Qt.quit()
        }
    }
}
