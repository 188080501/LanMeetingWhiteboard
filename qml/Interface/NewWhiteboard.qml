import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import "qrc:/MaterialUI/Interface/"

Rectangle {
    id: mainRectangle
    width: 320
    height: 548
    color: "#00000000"

    function show() {
        visible = true;
        openOrCloseAnimation.from = mainRectangle.height;
        openOrCloseAnimation.to = (mainRectangle.height - centerRectangle.height) / 2;
        openOrCloseAnimation.easing.type = Easing.OutQuart;
        openOrCloseAnimation.start();
    }

    function hide() {
        openOrCloseAnimation.from = (mainRectangle.height - centerRectangle.height) / 2;
        openOrCloseAnimation.to = mainRectangle.height;
        openOrCloseAnimation.easing.type = Easing.InSine;
        openOrCloseAnimation.start();
    }

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        id: centerRectangle
        x: parent.width - 300 / 2
        y: 96
        width: 300
        height: 330
        color: "#aaffffff"
        radius: 10
        anchors.horizontalCenter: parent.horizontalCenter

        PropertyAnimation {
            id: openOrCloseAnimation
            target: centerRectangle
            property: "y"
            duration: 600

            onStopped: {
                if(easing.type === Easing.InSine)
                {
                    mainRectangle.visible = false;
                }
            }
        }

        MaterialTextField {
            id: textFieldServerName
            x: 86
            y: 184
            width: 220
            height: 90
            anchors.verticalCenterOffset: -103
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: qsTr("画室名")
            characterLimit: 50

            Component.onCompleted: {
                text = JasonQtTool.deviceName() + qsTr("的画室");
            }
        }

        MaterialTextField {
            id: textFieldPassword
            x: 83
            y: 262
            width: 120
            height: 90
            anchors.verticalCenterOffset: -23
            anchors.horizontalCenterOffset: -50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: qsTr("密码")
            characterLimit: 20
            enabled: checkBoxPassword.checked
            input.echoMode: TextInput.Password
        }

        MaterialCheckBox {
            id: checkBoxPassword
            width: 100
            height: 50
            text: qsTr("使用密码")
            anchors.verticalCenterOffset: -18
            anchors.horizontalCenterOffset: 60
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        MaterialCheckBox {
            id: checkBoxClientEdit
            x: 76
            y: 316
            width: 220
            height: 50
            text: qsTr("允许其他用户编辑画布")
            anchors.verticalCenterOffset: 47
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            checked: true
        }

        MaterialButton {
            x: 62
            y: 400
            width: 80
            height: 40
            text: qsTr("取消")
            elevation: 1
            anchors.verticalCenterOffset: 115
            anchors.horizontalCenterOffset: -60
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            backgroundColor: "#ffffff"

            onClicked: {
                hide();
            }
        }

        MaterialButton {
            x: 185
            y: 405
            width: 80
            height: 40
            text: qsTr("确认")
            elevation: 1
            anchors.verticalCenterOffset: 115
            anchors.horizontalCenterOffset: 60
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            backgroundColor: materialUI.accentColor

            onClicked: {
                hide();
                mainCanvas.show();

                mainCanvas.edit = true;
                WhiteboardCanvasPainter.clearAll();
                WhiteboardSocket.startServer(textFieldServerName.text, ((checkBoxPassword.checked) ? (textFieldPassword.text) : ("")), checkBoxClientEdit.checked);
            }
        }
    }
}
