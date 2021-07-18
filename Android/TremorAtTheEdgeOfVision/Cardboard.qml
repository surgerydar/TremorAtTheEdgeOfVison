import QtQuick 2.15
import QtQuick.Controls 2.12

Item {
    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    ImageButton {
        width: 48
        height: width
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 16
        image: "/icons/close.png"
        onClicked: {
            stack.pop()
        }
    }

    Label {
        anchors.centerIn: parent
        text: "Cardboard"
    }
}
