import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    id: _container
    property string name
    property string hash
    property var container
    Switch {
        text: _container.name
        onClicked: {
            if(checked) {
                container.startApp(hash)
            } else {
                container.stopApp(hash)
            }
        }
    }
}
