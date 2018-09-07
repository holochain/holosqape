import QtQuick 2.11

Rectangle {
    id: root
    color: '#374ac7'
    width: 50
    signal consoleIconClicked()
    signal rootUISelected(string name)

    Column {
        id: column
        anchors.fill: parent
        padding: 8
        spacing: 8
        Repeater {
            model: Container.rootUINames
            Image {
                property string rootUIName: Container.rootUINames[index]
                source: 'file://'+Container.rootUIsDirectoryPath()+'/'+rootUIName+'/dock_icon.png'
                width: parent.width - parent.padding*2
                height: width

                //anchors.top: root.top

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.rootUISelected(rootUIName)
                }
            }
        }
    }

    Column {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        padding: column.padding

        Image {
            source: 'images/console.png'
            width: parent.width - column.padding*2
            height: width


            MouseArea {
                anchors.fill: parent
                onClicked: root.consoleIconClicked()
            }
        }

    }


}
