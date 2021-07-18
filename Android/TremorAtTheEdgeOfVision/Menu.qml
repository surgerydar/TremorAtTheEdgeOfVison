import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3

Item {
    Column {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16
        Image {
            width: parent.width
            source: "/icons/Home page title@4x.png"
        }

        Image {
            width: parent.width
            source: "/icons/VR GLASSES  Button @4x.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack.push("qrc:///SceneList.qml",{stereo:true})
                }
            }
        }

        Image {
            width: parent.width
            source: "/icons/360 2D VIDEO Button @4x.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack.push("qrc:///SceneList.qml",{stereo:false})
                }
            }
        }

        Image {
            width: parent.width
            source: "/icons/About Button @4x.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack.push("qrc:///About.qml")
                }
            }
        }

        Image {
            width: parent.width
            source: "/icons/HELP  Button @4x.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack.push("qrc:///Help.qml")
                }
            }
        }
        /*
        Button {
            text: "Add Site"
            onClicked: {
                SiteList.addSiteAtCurrentLocation()
            }
        }
        */
    }
    Connections {
        target: LocationManager
        function permissionRequiredChanged( permissionRequired ){
            if ( permissionRequired ) {
                LocationManager.requestPermissions()
            }
        }
    }
}
