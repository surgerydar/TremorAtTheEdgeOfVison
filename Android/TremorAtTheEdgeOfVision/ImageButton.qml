import QtQuick 2.0

Item {
    Image {
        id: icon
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parent.clicked()
        }
    }

    signal clicked()

    property alias image: icon.source
}
