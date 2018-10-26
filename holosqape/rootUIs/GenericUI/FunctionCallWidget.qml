import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


ColumnLayout {
    id: _this
    property var app
    property string zome
    property string capability
    property string fn
    property var parameters: app.parameter_names(zome, capability,fn)
    property var parameter_inputs: []

    Layout.fillWidth: true

    Component.onCompleted: {


        //parameters.placeholderText = as_string
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
            visible: parameters.length > 0

            Row {
                height: parent.height
                spacing: 15

                Repeater {
                    model: parameters

                    Row {
                        height: parent.height
                        property string parameter: parameters[index]
                        property var i: index
                        TextInput {
                            id: parameters_input
                            property string parameter: parent.parameter
                            width: max(text.length, parameter.length) * 8
                            height: parent.height

                            function max(x,y) {
                                if(x>y) return x; else return y;
                            }

                            property string placeholderText: "Enter text here..."

                            Component.onCompleted: {
                                _this.parameter_inputs.push(this)
                            }

                            Text {
                                text: parameter
                                color: "#aaa"
                                visible: !parameters_input.text
                            }
                        }

                        Text {
                            text: ","
                            font.pixelSize: 20
                            font.bold: true
                            visible: i < _this.parameters.length-1
                        }
                    }


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
                var parameter_names = app.parameter_names(zome, capability,fn)
                var json_string = "{"
                for(var i=0; i<parameter_inputs.length; i++) {
                    if(i>0) json_string +=","
                    json_string +="\"" + parameter_inputs[i].parameter + "\"";
                    json_string +=":";
                    json_string +="\"" + parameter_inputs[i].text + "\"";
                }
                json_string += "}"

                var result = app.call(zome, capability, fn, json_string)
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

        TextEdit {
            id: output
            text: "<out will be displayed here>"
            readOnly: true
            wrapMode: Text.WordWrap
            selectByMouse: true
        }

    }
}
