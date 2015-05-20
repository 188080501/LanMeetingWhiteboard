import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

Rectangle {
    id: mainRectangle

    width: 270
    height: 125
    color: "#44000000"
    visible: false

    property bool userScale: true;

    property var needCallbackOnOK;

    function show(titleText, messageText, callbackOnOK){
        showAnimation.start();
        showAnimation2.start();
        showNow(titleText, messageText, callbackOnOK);
    }

    function showNow(titleText, messageText, callbackOnOK){
        if(closeAnimation.running)
        {
            closeAnimation.stop();
        }

        mainRectangle.visible = true;
        mainRectangle.focus = true;
        labelTitle.text = (titleText) ? (titleText) : ("Title");
        labelMessage.text = (messageText) ? (messageText) : ("Message");
        needCallbackOnOK = (callbackOnOK) ? (callbackOnOK) : (null);
    }

    function hide(){
        closeAnimation.start();
    }

    function hideNow(){
        if(showAnimation.running)
        {
            showAnimation.stop();
        }
        if(showAnimation2.running)
        {
            showAnimation2.stop();
        }

        mainRectangle.visible = false;
    }

    MouseArea {
        anchors.fill: parent
    }

    Keys.onEnterPressed: {
        if(buttonOK.enabled)
        {
            buttonOK.clicked();
        }
    }
    Keys.onReturnPressed: {
        if(buttonOK.enabled)
        {
            buttonOK.clicked();
        }
    }

    PropertyAnimation {
        id: showAnimation;
        target: mainRectangle;
        property: "opacity";
        from: 0.0;
        to: 1.0;
        duration: 200;
        easing.type: Easing.InCubic
    }

    PropertyAnimation {
        id: showAnimation2;
        target: mainRectangle;
        property: "scale";
        from: 1.3;
        to: 1.0;
        duration: 300;
        easing.type: Easing.OutCubic
    }

    PropertyAnimation {
        id: closeAnimation;
        target: mainRectangle;
        property: "opacity";
        to: 0.0;
        duration: 200;
        easing.type: Easing.OutCubic

        onStopped: {
            mainRectangle.visible = false;
        }
    }

    Rectangle {
        id: centerRectangle
        width: 270;
        height: 125;
        color: "#00000000"
        radius: 10
        border.width: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        scale: (userScale) ? (JasonQtTool.calculateDoubleScale(mainRectangle.width, mainRectangle.height, 320, 568)) : (1.0);

        Image {
            anchors.fill: parent;
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/iOSStyleWidgetsResources/MessageBoxAlert.png"
        }

        Text {
            id: labelTitle
            x: 82
            y: 17
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 17
            font.bold: true
        }

        Text {
            id: labelMessage
            x: 87
            y: 45
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 13
        }

        Button {
            id: buttonOK
            x: 0
            y: 89
            width: parent.width
            height: 45
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter

            style: ButtonStyle {
                background: Rectangle {
                    color: (control.pressed) ? ("#80e0e0e0") : ("#00000000")
                }

                label: Text {
                    text: qsTr("OK")
                    font.pixelSize: 17
                    color: "#157efb"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            onClicked: {
                if(needCallbackOnOK)
                {
                    needCallbackOnOK();
                }
            }
        }
    }
}
