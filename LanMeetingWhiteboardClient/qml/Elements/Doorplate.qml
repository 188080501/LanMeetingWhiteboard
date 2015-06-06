import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2

Rectangle {
    id: mainRectangle
    width: 300
    height: 50
    color: "#99ffffff"
    border.color: "#a1a1a1"
    border.width: 2
    radius: 5

    function showCanvas() {
        WhiteboardCanvasPainter.clearAll();
        WhiteboardSocket.startClient(ipText);
        mainCanvas.show();
        mainCanvas.edit = WhiteboardList.server(ipText)["clientEdit"];
    }

    Button {
        anchors.fill: parent
        style: ButtonStyle {
            background: Rectangle { color: (control.pressed) ? ("#22000000") : ("#00000000") }
        }

        onClicked: {
            if(password != "")
            {
                mainWhiteboardListRefreshFlag = false;
                materialUI.showDialogPrompt(qsTr("请输入密码"), qsTr("这个房间需要密码才能访问"), qsTr("画室密码"), "",
                                            function() {
                                                mainWhiteboardListRefreshFlag = true;
                                            },
                                            function(currentPassword){
                                                if(password !== currentPassword)
                                                {
                                                    materialUI.showDialogAlert(qsTr("密码错误"), qsTr("请确认您的密码"), null);
                                                    return;
                                                }
                                                showCanvas()
                                                mainWhiteboardListRefreshFlag = true;
                                            });
            }
            else
            {
                showCanvas();
            }
        }
    }

    Text {
        id: textIP
        y: 13
        width: 130
        text: qsTr("IP：") + ipText
        clip: true
        anchors.verticalCenterOffset: -9
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 166
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 13
    }

    Text {
        id: textHostName
        y: 17
        width: 130
        text: qsTr("主机：") + HostNameText
        clip: true
        anchors.left: parent.left
        anchors.leftMargin: 166
        anchors.verticalCenterOffset: 9
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 13
    }

    Text {
        id: textServerName
        x: 8
        y: 16
        width: 152
        height: 40
        text: serverNameText
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        wrapMode: Text.WrapAnywhere
        clip: true
        font.pixelSize: 15
    }
}
