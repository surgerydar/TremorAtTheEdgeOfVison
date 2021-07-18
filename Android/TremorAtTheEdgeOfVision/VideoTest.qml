import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtMultimedia 5.12

Item {

    Video {
        id: video
        anchors.fill: parent
        autoLoad: true
        autoPlay: true
        fillMode: VideoOutput.PreserveAspectFit
        onErrorChanged: {
            console.log(`error=${error}`)
        }
        onErrorStringChanged: {
            console.log(`error=${errorString}`)
        }
        onStatusChanged: {
            console.log(`status=${status}`)
        }

        onHasAudioChanged: {
            console.log(`hasAudio=${hasAudio}`)
        }
        onHasVideoChanged: {
            console.log(`hasVideo=${hasVideo}`)
        }
    }
    Row {
        anchors.centerIn: parent
        Button {
            text: "Back"
            onClicked: {
                stack.pop()
            }
        }
        /*
        Button {
            text: "Open"
            onClicked: {
                //chooser.open()
                video.source = `file://${SystemUtils.documentPath('3D360.m4v')}`
            }
        }
        */
    }
    property alias source: video.source
}
