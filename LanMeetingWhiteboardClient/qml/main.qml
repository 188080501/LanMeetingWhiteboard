import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import "qrc:/qml/Elements/"
import "qrc:/qml/Interface/"
import "qrc:/MaterialUI/Element/"
import "qrc:/MaterialUI/Interface/"

ApplicationWindow {
    id: applicationWindow
    title: qsTr("Lan meting whiteboard")
    width: 320
    height: 548
    visible: true

    Image {
        anchors.fill: parent
        source: "qrc:/UI/Background.png"
    }

    Text {
        id: textList
        x: 168
        y: 18
        text: qsTr("画室列表")
        font.bold: true
        anchors.horizontalCenterOffset: 1
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 18
    }

    DoorplateList {
        id: mainDoorplateList
        x: 10
        y: 50
        width: parent.width - 20
        height: parent.height - 55
    }

    MaterialActionButton {
        width: 56
        height: 56
        anchors.rightMargin: 15
        anchors.bottomMargin: 15
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        backgroundColor: materialUI.accentColor
        text: "\uf067"
        textFontFamily: mainIconFont.name
        textSize: 18

        onClicked: {
            mainNewWhiteboard.show();
        }
    }

    MaterialButton {
        width: 50
        height: 53
        text: "\uf013"
        textFontFamily: mainIconFont.name
        textSize: 20
        anchors.top: parent.top
        anchors.right: parent.right
        elevation: 0

        onClicked: {
            mainSettings.show();
        }
    }

    NewWhiteboard {
        id: mainNewWhiteboard
        width: parent.width
        height: parent.height
        visible: false
    }

    Settings {
        id: mainSettings
        width: parent.width
        height: parent.height
        visible: false
    }

    Canvas {
        id: mainCanvas
        width: parent.width
        height: parent.height
        visible: false
        onExitCallback: (function() {
            mainCanvas.hide();
            WhiteboardSocket.stop();
        })
    }

    FontLoader {
        id: mainIconFont
        source: "qrc:/Font/fontawesome-webfont-4-3-0.ttf"
    }

    MaterialUI {
        id: materialUI
    }
}
