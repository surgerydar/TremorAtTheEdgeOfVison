import QtQuick 2.0
import QtQuick.Controls 2.12
import uk.co.soda.VR 1.0

Item {
    id: container
    Image {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 16
        //fillMode: Image.PreserveAspectFit
        source: "/icons/Home page title@4x.png"
    }

    Column {
        anchors.centerIn: parent
        visible: !( SiteProximityManager.offsite && SiteProximityManager.isLocated )
        spacing: 16
        Label {
            horizontalAlignment: Label.AlignHCenter
            font.family: bodyFont.name
            font.pointSize: 18
            text: "Searching for locations..."
        }
        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    ListView {
        id: siteList
        visible: SiteProximityManager.offsite && SiteProximityManager.isLocated
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.bottom: backButton.top
        anchors.right: parent.right
        anchors.margins: 16
        spacing: 24
        interactive: false
        model: ListModel {}
        delegate: Item {
            height: 48
            width: siteList.width - 32
            Text {
                anchors.fill: parent
                font.family: bodyFont.name
                font.pointSize: 24
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: "blue"
                text: model.notes
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack.push("qrc:///Pano.qml", {source:model.sequence,horizontalOffset:model.horizontalOffset,stereo:stereo})
                }
            }
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

    Component.onCompleted: {
        siteList.model.clear()
        let count = SiteList.siteCount()
        for ( var i = 0; i < count; i++ ) {
            let site = SiteList.getDisplaySite(i)
            siteList.model.append(site)
        }
        LocationManager.start()
        SiteProximityManager.start()
    }
    Component.onDestruction: {
        SiteProximityManager.stop()
        LocationManager.stop()
    }

    Connections {
        target: SiteProximityManager
        function onOffsiteChanged( offsite ) {
            console.log(`SceneList : offsite changed : ${offsite}`)
            if ( !offsite ) {
                stack.push("qrc:///ProximityPano.qml", {stereo:stereo})
            }
        }
    }

    property bool stereo: false
}
