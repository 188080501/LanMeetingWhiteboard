import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import "qrc:/qml/Elements/"

Rectangle {
    width: 300
    height: 503
    color: "#00000000"
    clip: true

    property bool mainWhiteboardListRefreshFlag: true

    function refreshWhiteboardList() {
        if(!mainWhiteboardListRefreshFlag) { return; }

        listModel.clear();
        var list = WhiteboardList.availableServers();
        for(var index = 0; index < list.length; index++)
        {
            listModel.append({
                                 "ipText": list[index]["ip"],
                                 "HostNameText": list[index]["hostName"],
                                 "password": list[index]["password"],
                                 "serverNameText": list[index]["serverName"]
                             });
        }
    }

    onVisibleChanged: {
        if(visible)
        {
            refreshWhiteboardList();
        }
    }

    Component.onCompleted: {
        refreshWhiteboardList();
    }

    Connections {
        target: WhiteboardList
        onServerListChange: {
            if(visible)
            {
                refreshWhiteboardList();
            }
        }
        onBeginFail: {
            materialUI.showDialogAlert(qsTr("初始化失败"), qsTr("请确认您的网络环境为局域网并重启APP"), null);
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: ListModel {
            id: listModel
        }
        delegate: Rectangle {
            x: (listView.width - 300) / 2
            width: 300
            height: 55
            color: "#00000000"

            Doorplate {
                y: 2
            }
        }
    }
}
