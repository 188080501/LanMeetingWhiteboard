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

    property bool isCompleted: false

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

    onVisibleChanged: {
        if(!isCompleted) { return; }

        if(visible)
        {
            switchAutoHideToolBar.checked = WhiteSettings.value("autoHideToolBar");
            switchUseVop.checked = WhiteSettings.value("useVop");
        }
        else
        {
            WhiteSettings.setValue("autoHideToolBar", switchAutoHideToolBar.checked);
            WhiteSettings.setValue("useVop", switchUseVop.checked);
        }
    }

    Component.onCompleted: {
        isCompleted = true;

        WhiteSettings.value("autoHideToolBar", true);
        WhiteSettings.value("useVop", true);
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            hide();
        }
    }

    Rectangle {
        id: centerRectangle
        x: parent.width - 300 / 2
        y: (mainRectangle.height - height) / 2
        width: 300
        height: 330
        color: "#aaffffff"
        radius: 10
        anchors.horizontalCenter: parent.horizontalCenter

        MouseArea {
            anchors.fill: parent
        }

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

        Text {
            x: 54
            y: 86
            width: 120
            height: 50
            text: "自动隐藏边栏"
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 15
            horizontalAlignment: Text.AlignHCenter
        }

        MaterialSwitch {
            id: switchAutoHideToolBar
            x: 191
            y: 86
            width: 80
            height: 50
        }

        Text {
            x: 54
            y: 160
            width: 120
            height: 50
            text: "开启语音助手"
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 15
            horizontalAlignment: Text.AlignHCenter
        }

        MaterialSwitch {
            id: switchUseVop
            x: 191
            y: 160
            width: 80
            height: 50
        }

        MaterialButton {
            x: 62
            y: 400
            width: 80
            height: 40
            text: qsTr("退出")
            elevation: 1
            anchors.verticalCenterOffset: 115
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            backgroundColor: "#ffffff"

            onClicked: {
                hide();
            }
        }
    }
}
