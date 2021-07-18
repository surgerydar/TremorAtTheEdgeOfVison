import QtQuick 2.0
import QtQuick.Controls 2.12
Item {
    id: container
    Image {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right:parent.right
        anchors.margins: 16
        source: "/icons/About page title@4x.png"
    }
    ScrollView {
        anchors.top:header.bottom
        anchors.left:parent.left
        anchors.bottom:logos.top
        anchors.right:parent.right
        anchors.margins: 16
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        Text {
            width: container.width - 32
            wrapMode: Text.Wrap
            textFormat: Text.RichText
            font.family: bodyFont.name
            font.pointSize: 18
            text: SystemUtils.readFile(":/data/help.html")
            onLinkActivated: {
                Qt.openUrlExternally(link)
            }
        }
    }

    Row {
        id: logos
        anchors.bottom: backButton.top
        anchors.left: parent.left
        anchors.right:parent.right
        anchors.margins: 16
        spacing: 32

        Image {
            width: (parent.width / 2) - 16
            fillMode: Image.PreserveAspectFit
            source: "/icons/Metal logo@4x.png"
        }
        Image {
            width: (parent.width / 2) - 16
            fillMode: Image.PreserveAspectFit
            source: "/icons/ESTUARY2021_LOGO_BLACK_BLACK_SCREEN_RGB@4x.png"
        }
    }
    Image {
        id: backButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 16
        width: ( parent.width / 2 ) - 32
        source: "/icons/BACK Button @4x.png"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                stack.pop()
            }
        }
    }
}
