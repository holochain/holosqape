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
                call_button.enabled = true
            } else {
                container.stopApp(hash)
                call_button.enabled = false
            }
        }
    }
    Button {
        id: call_button
        text: 'call: test_zome/test_cap/main()'
        enabled: false
        onClicked: {
            var result = container.call(hash, 'test_zome', 'test_cap', 'main', '')
            output.text = result
        }
    }
    Text {
        id: output
        text: ""
    }
}
