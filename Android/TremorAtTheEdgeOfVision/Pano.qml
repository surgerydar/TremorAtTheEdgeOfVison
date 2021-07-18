import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import uk.co.soda.VR 1.0

Item {
    /*
    Rectangle {
        anchors.fill: parent
        color: "white"
    }
    */
    VRVideo {
        id: video
        anchors.fill: parent
        onPlaying: {
            console.log( 'VRVideo.playing' )
        }
        onPaused: {
            console.log( 'VRVideo.paused' )
        }
        onStopped: {
            console.log( 'VRVideo.stopped' )
            SiteProximityManager.sitePlayed()
            stack.pop()
        }
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
                if ( stack.depth < 4 ) {
                    stack.pop()
                } else {
                    stack.pop(null)
                }
            }
        }
    }

    function stop() {
        video.stop();
    }

    Component.onCompleted: {
        //console.log(`Pano stackDepth=${stack.depth}`)
        if ( stack.depth < 4 ) {
            SystemUtils.preventDeviceSleep()
        }
    }

    Component.onDestruction: {
        video.stop()
        if ( stack.depth < 3 ) { // this item has already been removed from stack
            SystemUtils.allowDeviceSleep()
        }
    }

    Timer {
         interval: 500
         running: source.length > 0
         repeat: false
         onTriggered: {
             let filePath = SystemUtils.documentPath(source)
             if ( SystemUtils.fileExists(filePath) ) {
                 let url = `file://${filePath}`
                 console.log(`Pano : playing ${url}`)
                 video.play(url)
             } else {
                 console.log(`Pano : unable to find file ${filePath}`)
             }
         }
    }
    property string source: ""
    property real horizontalOffset: video.horizontalOffset
    property alias stereo: video.stereo
}
