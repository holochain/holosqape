import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


ColumnLayout {
    id: _this
    property var app
    property string zome
    property string capability
    property string fn


    Layout.fillWidth: true

    Component.onCompleted: {
        var parameter_names = app.parameter_names(zome, capability,fn)
        var as_string = ""
        for(var i=0; i<parameter_names.length; i++) {
            as_string += parameter_names[i]
            if(i<parameter_names.length-1)
                as_string += ", "
        }
        parameters.placeholderText = as_string
    }

    RowLayout {
        Layout.fillHeight: false
        Layout.fillWidth: true
        Text {
            id: name
            text: zome + "/" + capability + "/" + fn +"("
            font.pixelSize: 20
            font.bold: true
        }

        Rectangle {
            Layout.fillWidth: true
            color: "white"
            border.width: 1
            Layout.minimumHeight: 23

            TextInput {
                id: parameters
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.topMargin: 2

                property string placeholderText: "Enter text here..."

                Text {
                    text: parameters.placeholderText
                    color: "#aaa"
                    visible: !parameters.text
                }
            }
        }


        Text {
            text: ")"
            font.pixelSize: 20
            font.bold: true
        }

        Button {
            id: button
            text: "Run"
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
        Layout.preferredHeight: 40

        Text {
            id: output
            text: "<out will be displayed here>"
        }
    }
}
