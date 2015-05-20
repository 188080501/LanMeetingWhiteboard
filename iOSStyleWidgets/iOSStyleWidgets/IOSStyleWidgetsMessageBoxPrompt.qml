import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Rectangle {
    id: mainRectangle

    width: 270
    height: 163
    color: "#44000000"
    visible: false

    property bool userScale: true;

    property var needCallbackOnCancel;
    property var needCallbackOnConfirm;

    function show(titleText, messageText, placeholderText, currentText, callbackOnCancel, callbackOnConfirm) {
        showAnimation.start();
        showAnimation2.start();
        showNow(titleText, messageText, placeholderText, currentText, callbackOnCancel, callbackOnConfirm);
    }

    function showNow(titleText, messageText, placeholderText, currentText, callbackOnCancel, callbackOnConfirm) {
        if(closeAnimation.running)
        {
            closeAnimation.stop();
        }

        mainRectangle.visible = true;
        labelTitle.text = (titleText) ? (titleText) : ("Title");
        labelMessage.text = (messageText) ? (messageText) : ("Message");
        textField.focus = true;
        textField.placeholderText = (placeholderText) ? (placeholderText) : ("placeholderText");
        textField.text = (currentText) ? (currentText) : ("");
        needCallbackOnCancel = (callbackOnCancel) ? (callbackOnCancel) : (null);
        needCallbackOnConfirm = (callbackOnConfirm) ? (callbackOnConfirm) : (null);
    }

    function hide() {
        closeAnimation.start();
    }

    function hideNow() {
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
        width: 270
        height: 163;
        color: "#00000000"
        radius: 10
        border.width: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        scale: (userScale) ? (JasonQtTool.calculateDoubleScale(mainRectangle.width, mainRectangle.height, 320, 568)) : (1.0);

        Image {
            anchors.fill: parent;
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/iOSStyleWidgetsResources/MessageBoxPrompt.png"
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
            y: 43
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 13
        }

        TextField {
            id: textField
            x: 17
            y: 79
            width: 237
            height: 24
            textColor: "#000000"
            font.pixelSize: 13

            style: TextFieldStyle {
                background: Rectangle {
                    border.color: "#000000"
                    border.width: 1
                    color: "#ffffff"
                }
            }

            Keys.onEscapePressed: {
                if(buttonCancel.enabled)
                {
                    buttonCancel.clicked();
                }
            }

            Keys.onEnterPressed: {
                if(buttonConfirm.enabled)
                {
                    buttonConfirm.clicked();
                }
            }
            Keys.onReturnPressed: {
                if(buttonConfirm.enabled)
                {
                    buttonConfirm.clicked();
                }
            }
        }

        Button {
            id: buttonCancel
            y: 136
            width: parent.width / 2
            height: 45
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -1

            style: ButtonStyle {
                background: Rectangle {
                    color: (buttonCancel.pressed) ? ("#80e0e0e0") : ("#00000000")
                }

                label: Text {
                    text: qsTr("Cancel")
                    font.pixelSize: 17
                    color: "#157efb"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            onClicked: {
                if(needCallbackOnCancel)
                {
                    needCallbackOnCancel();
                }
            }
        }

        Button {
            id: buttonConfirm
            x: 135
            y: 136
            width: parent.width / 2
            height: 45
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -1
            enabled: textField.text

            style: ButtonStyle {
                background: Rectangle {
                    color: (buttonConfirm.pressed) ? ("#80e0e0e0") : ("#00000000")
                }

                label: Text {
                    text: qsTr("Confirm")
                    font.pixelSize: 17
                    color: (buttonConfirm.enabled) ? ("#157efb") : ("#8f8f8f")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            onClicked: {
                if(needCallbackOnConfirm)
                {
                    needCallbackOnConfirm(textField.text);
                }
            }
        }
    }
}
