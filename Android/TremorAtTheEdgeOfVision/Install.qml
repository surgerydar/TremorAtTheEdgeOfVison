import QtQuick 2.15
import QtQuick.Controls 2.12
import QtMultimedia 5.12

Item {
    Item {
        id: videoContainer
        anchors.fill: parent
        anchors.margins: 16
        visible: !progressContainer.visible
        Label {
            id: videoTitle
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: Label.AlignHCenter
            font.family: bodyFont.name
            font.pointSize: 24
            text: "Tremor at the Edge of Vision<br/>Can you see a video below?"
        }

        Rectangle {
            anchors.fill: testVideo
            color: "transparent"
            border.color: "black"
            border.width: 1
        }

        Label {
            anchors.centerIn: testVideo
            font.family: bodyFont.name
            font.pointSize: 18
            text: "No Video"
        }

        Video {
            id: testVideo
            anchors.top : videoTitle.bottom
            anchors.left: parent.left
            anchors.bottom : videoChoice.top
            anchors.right: parent.right
            anchors.margins: 8
            autoPlay: true
            fillMode: VideoOutput.PreserveAspectFit
            source: "https://soda.co.uk/tremorattheedge/Scene05_4seconds_3840x3840_HVEC_10bit_10Mbps.mp4"
        }

        Row {
            id: videoChoice
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 8
            Button {
                text: "Yes"
                font.family: bodyFont.name
                onClicked: {
                    testVideo.stop()
                    downloadHiRes = true
                    if( install() ) {
                        queueNextDownload()
                    } else {
                        stack.replace(null,"qrc:///Menu.qml")
                    }
                }
            }
            Button {
                text: "No"
                font.family: bodyFont.name
                onClicked: {
                    testVideo.stop()
                    downloadHiRes = false
                    if( install() ) {
                        queueNextDownload()
                    } else {
                        stack.replace(null,"qrc:///Menu.qml")
                    }
                }
            }
        }
    }
    Item {
        id: progressContainer
        anchors.fill: parent
        visible: false


        Column {
            id: installProgress
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 16
            spacing: 16
            Label {
                width: parent.width
                horizontalAlignment: Label.AlignHCenter
                font.family: bodyFont.name
                font.pointSize: 24
                text: "Tremor at the Edge of Vision<br/>Installing Media"
            }
            ProgressBar {
                id: progressBar
                anchors.left: parent.left
                anchors.right: parent.right
                from: 0
                to: 100
            }
            Label {
                id: progressInfo
                width: parent.width
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.NoWrap
                font.family: bodyFont.name
            }
        }
    }

    Component.onCompleted: {
        SystemUtils.preventDeviceSleep()
    }
    Component.onDestruction: {
        SystemUtils.allowDeviceSleep()
    }

    Connections {
        target: Downloader
        function onProgress(source,destination,total,current,message) {
            let itemProgress = current / total
            progressBar.value = 100 * itemProgress
        }
        function onError(source,destination,message) {
            progressInfo.text = message
            console.log(`Install : download error ${message}`)
            queueNextDownload();
        }
        function onDone(source,destination) {
            if ( downloadQueue.length > 0 ) {
                console.log('Install : queueing next download')
                queueNextDownload()
                //progressBar.value = ((downloadQueue.length-1) / downloadCount)*100
                progressInfo.text = `${downloadCount-downloadQueue.length} of ${downloadCount}`
            } else {
                AudioPlayer.reload();
                console.log('Install : done')
                progressContainer.visible = false
                stack.replace(null,"qrc:///Menu.qml")
            }
        }
    }

    function install() {
        let loRes = [
            "Scene01_TEST4_2400x2400_HVEC_10bit_10Mbps.mp4",
            "Scene02_TEST1_2400x2400_H264_10bit_10Mbps.mp4",
            "Scene03_RC1_2400x2400_H264_10bit_10Mbps.mp4",
            "Scene03B_RC1_2400x2400_H264_10bit_10Mbps.mp4",
            "Scene04_RC1_2400x2400_H264_10bit_10Mbps.mp4",
            "Scene05_RC1_2400x2400_H264_10bit_10Mbps.mp4",
            "Scene06_RC3_2400x2400_H264_10bit_10Mbps.mp4"
        ]

        let hiRes = [
            "Scene01_RC2_3840x3840_HVEC_10bit_10Mbps.mp4",
            "Scene02_RC1_3840x3840_HVEC_10bit_10Mbps.mp4",
            "Scene03_RC1_3840x3840_HVEC_10bit_10Mbps.mp4",
            "Scene03B_RC1_3840x3840_HVEC_10bit_10Mbps.mp4",
            "Scene04_RC1_3840x3840_HVEC_10bit_10Mbps.mp4",
            "Scene05_RC1_3840x3840_HVEC_10bit_10Mbps.mp4",
            "Scene06_RC3_3840x3840_HVEC_10bit_10Mbps.mp4"
        ]
        var i
        var filePath
        var fileUrl
        var videoList = downloadHiRes ? hiRes : loRes
        for ( i = 0; i < videoList.length; i++ ) {
            filePath = SystemUtils.documentPath(`Scene${i+1}.mp4`)
            if ( !SystemUtils.fileExists(filePath) ) {
                fileUrl = `https://soda.co.uk/tremorattheedge/${videoList[i]}`
                downloadQueue.push({
                                       source:fileUrl,
                                       target:filePath
                                   })

            }
        }
        let audioList = [
                "environmental_curtain_up_AAC.m4a",
                "In_Blue_background_track_AAC.m4a",
                "Scene_07_Guitar_walk_home_AAC.m4a"
            ]
        for ( i = 0; i < audioList.length; i++ ) {
            filePath = SystemUtils.documentPath(audioList[i])
            if ( !SystemUtils.fileExists(filePath) ) {
                fileUrl = `https://soda.co.uk/tremorattheedge/${audioList[i]}`
                downloadQueue.push({
                                       source:fileUrl,
                                       target:filePath
                                   })

            }
        }
        if ( downloadQueue.length > 0 ) {
            downloadCount = downloadQueue.length
            return true;
        }
        return false;
    }
    function queueNextDownload() {
        if ( downloadQueue.length > 0 ) {
            let download = downloadQueue.pop()
            progressBar.value = 0
            progressInfo.text = `${downloadCount-downloadQueue.length} of ${downloadCount}`
            Downloader.download(download.source,download.target)
            progressContainer.visible = true
        } else {
            if( install() ) {
                queueNextDownload()
                console.log('Install : attempting reinstall')
            } else {
                console.log('Install : done')
                stack.replace(null,"qrc:///Menu.qml")
            }
        }
    }

    property bool downloadHiRes: false;
    property var fileList: SiteList.getFileList()
    property var downloadQueue: []
    property int downloadCount: 0

}
