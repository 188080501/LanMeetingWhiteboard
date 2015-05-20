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
            background: Rectangle { color: "#00000000" }
        }

        onClicked: {
            if(password != "")
            {
                mainMessageBoxPrompt.show(qsTr("请输入密码"), qsTr("这个房间需要密码才能访问"), qsTr("画室密码"), "",
                                          mainMessageBoxPrompt.hide,
                                          function(currentPassword){
                                              mainMessageBoxPrompt.hide();

                                              if(password !== currentPassword)
                                              {
                                                  mainMessageBoxAlert.show(qsTr("密码错误"), qsTr("请确认您的密码"), mainMessageBoxAlert.hide);
                                                  return;
                                              }

                                              showCanvas();
                                          })
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
        text: qsTr("IP：") + ipText
        anchors.verticalCenterOffset: -9
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 171
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 13
    }

    Text {
        id: textHostName
        y: 17
        text: qsTr("主机：") + HostNameText
        anchors.left: parent.left
        anchors.leftMargin: 171
        anchors.verticalCenterOffset: 9
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 13
    }

    Text {
        id: textServerName
        x: 12
        y: 16
        text: serverNameText
        font.pixelSize: 15
    }
}
