import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


Rectangle {
    id: item1
    property var app
    property string zome
    property string capability
    property string fn


    Layout.fillWidth: true

    color: "red"
    radius: 1
    border.width: 2

    Component.onCompleted: {

    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: false
            Layout.fillWidth: true
            Text {
                id: name
                text: zome + "/" + capability + "/" + fn +"()"
                anchors.left: parent.left
                font.pixelSize: 20
            }

            TextInput {
                id: parameters
                anchors.left: name.right
                Layout.fillWidth: true
            }

            Button {
                id: button
                text: "Run"
                anchors.left: parameters.right
                anchors.right: parent.right
                anchors.top: parent.top
                onClicked: {
                    var result = app.call(zome, capability, fn, parameters.text)
                    output.text = result
                }
            }
        }

        Rectangle {
            color: "white"
            border.width: 1
            Layout.minimumHeight: 23
            Layout.fillWidth: true
            Layout.preferredHeight: 20

            Text {
                id: output
                text: "<out will be displayed here>"
            }
        }
    }
}
