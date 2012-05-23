// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import Social 1.0

Rectangle {
    width: 360
    height: 360
    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }

    Twitter{
        id: twitter
        consumerKey: "1RWjs9F86SZIWULRwlDWEw"
        consumerSecret: "8f2m0BcEewdvH4NKjA6c5q7snmdVM3zcSz7RsGcm8"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            twitter.testUploadImage();
        }
    }
}
