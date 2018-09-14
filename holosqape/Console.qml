import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4

Rectangle {
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

}
