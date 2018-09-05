import QtQuick 2.0

Rectangle {
    id: root
    color: 'red'
    width: 50
    signal consoleIconClicked()
    signal rootUISelected(string name)


    Image {
        source: 'images/blank.svg'
        width: parent.width
        height: parent.width

        anchors.top: root.top

        MouseArea {
            anchors.fill: parent
            onClicked: root.rootUISelected('GenericUI')
        }
    }

    Image {
        source: 'images/console.png'
        width: parent.width
        height: parent.width

        anchors.bottom: root.bottom

        MouseArea {
            anchors.fill: parent
            onClicked: root.consoleIconClicked()
        }
    }
}
