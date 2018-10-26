import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    id: _app_widget
    property string name
    property string hash
    property var container
    property var app

    Layout.fillWidth: true

    Switch {
        text: _app_widget.name
        font.pixelSize: 20
        font.bold: true
        checked: true
        onClicked: {
            if(checked) {
                app.start()
            } else {
                app.stop()
            }
        }
    }

    Component.onCompleted: {
        var zome_names = app.zome_names()
        for(var i=0; i<zome_names.length; i++) {
            var zome_name = zome_names[i]
            var capabilities = app.capabilities_for_zome(zome_name)
            for(var j=0; j<capabilities.length; j++) {
                var cap_name = capabilities[j]
                var functions = app.functions_for_zome_capability(zome_name, cap_name)
                for(var k=0; k<functions.length; k++) {
                    var fn_name = functions[k]
                    var component = Qt.createComponent("FunctionCallWidget.qml");
                    var object = component.createObject(this,
                        {
                            app: app,
                            zome: zome_name,
                            capability: cap_name,
                            fn: fn_name
                        }
                    )
                }
            }
        }
    }
}
