import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import uk.co.soda.VR 1.0

Item {
    id: container
    Rectangle {
        id: cover
        anchors.fill: parent
        color: "black"
    }
    /*
    Rectangle {
        anchors.fill: shortest
        anchors.margins: -8
        color: "white"
    }
    Label {
        id: shortest
        anchors.centerIn: parent
        horizontalAlignment: Label.AlignHCenter
        verticalAlignment: Label.AlignVCenter
        text: "nearest:" + SiteProximityManager.shortest
    }
    */
    Item {
        width: 64
        height: 64
        anchors.top: parent.top
        anchors.left: parent.left
        Image {
            width: 32
            height: 32
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            source: "/icons/LeftChevron@4x.png"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                stack.pop(null)
            }
        }
    }

    Component.onCompleted: {
        console.log('ProximityPano.Component.onCompleted')
        SystemUtils.preventDeviceSleep()
        console.log(`ProximityPano.Component.onCompleted : activeSequence=${SiteProximityManager.activeSequence}`)
        if ( SiteProximityManager.activeSequence.length > 0 ) {
            playVideo(SiteProximityManager.activeSequence)
        }
    }

    Component.onDestruction: {
        console.log('ProximityPano.Component.onDestruction')
        AudioPlayer.stopAll()
        SystemUtils.allowDeviceSleep()
    }

    Connections {
        target: SiteProximityManager
        function onActiveSequenceChanged(sequence) {
            if ( sequence.length === 0 ) {
                console.log(`ProximityPano : stopping video`)
                // if the video plays through player could be non null but still invalid
                try {
                    if ( stack.currentItem === player ) {
                        if ( player != null ) {
                            player.stop()
                            player = null
                        }
                    }
                } catch( error ) {
                }
                player = null
            } else {
                console.log(`ProximityPano : playing video "${SiteProximityManager.activeSequence}"`)
                playVideo(SiteProximityManager.activeSequence)
            }
        }
        function onOffsiteChanged( offsite ) {
            console.log(`ProximityPano : offsite changed : ${offsite}`)
            if ( offsite ) {
                stack.pop()
            }
        }
    }

    function playVideo(sequence) {
        if ( sequence.length > 0 ) {
            let filePath = SystemUtils.documentPath(sequence)
            if ( SystemUtils.fileExists(filePath) ) {
                let url = `file://${filePath}`
                console.log(`ProximityPano : playing ${url}`)
                player = stack.push("qrc:///Pano.qml", {source:sequence,horizontalOffset:SiteProximityManager.activeHorizontalOffset,stereo:stereo})
            } else {
                console.log(`ProximityPano : unable to find file ${filePath}`)
            }
        }
    }
    property bool stereo: false
    property var player: null
}
