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
        consumerKey: "W3f50lSJiI00CDVg6UwhxQ"
        consumerSecret: "CHPWgrNu1wanwvSXax3b7FQarXBgVeZXUFysD4yZKXg"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            twitter.testResource();
        }
    }
}
