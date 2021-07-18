import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Tremor at the Edge")

    FontLoader {
        id: bodyFont
        source: "/fonts/TT2020StyleE-Regular.ttf"
        onStatusChanged: {
            if (status == FontLoader.Ready) {
                console.log('bodyFont ready : name' + regularFont.name );
            } else if (status == FontLoader.Error ) {
                console.log('bodyFont error' );
            }
        }
    }

    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        source: "/icons/HOME SCREEN@4x.png"
    }

    StackView {
        id:stack
        anchors.fill: parent
        Component.onCompleted: {
            var installed = true;
            let videoList = SiteList.getFileList()
            var i
            var filePath
            for ( i = 0; i < videoList.length; i++ ) {
                filePath = SystemUtils.documentPath(videoList[i])
                if ( !SystemUtils.fileExists(filePath) ) {
                    installed = false;
                    break;
                }
            }
            if ( installed ) {
                let audioList = [
                        "environmental_curtain_up_AAC.m4a",
                        "In_Blue_background_track_AAC.m4a",
                        "Scene_07_Guitar_walk_home_AAC.m4a"
                    ]
                for ( i = 0; i < audioList.length; i++ ) {
                    filePath = SystemUtils.documentPath(audioList[i])
                    if ( !SystemUtils.fileExists(filePath) ) {
                        installed = false;
                        break;
                    }
                }
            }
            if ( installed ) {
                push("qrc:///Menu.qml")
            } else {
                push("qrc:///Install.qml")
            }
        }
    }
    property int mappingX: 0
    property int mappingY: 2
    onMappingXChanged: {
        MotionManager.setMapping(mappingX,mappingY)
    }
    onMappingYChanged: {
        MotionManager.setMapping(mappingX,mappingY)
    }
}
