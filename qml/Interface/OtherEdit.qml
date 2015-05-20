import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import "qrc:/qml/Elements/"

Rectangle {
    id: mainRectangle
    width: 50
    height: 300
    color: "#00000000"

    property var onExitCallback

    Rectangle{
        anchors.fill: centerRectangle
        color: "#ddffffff"
        clip: true

        property var target: centerRectangle

        FastBlur {
            id: blur
            source: parent.target
            width: source.width;
            height: source.height
            radius: 20
        }

        onXChanged: setBlurPosition();
        onYChanged: setBlurPosition();
        Component.onCompleted: setBlurPosition();
        function setBlurPosition(){
            blur.x = target.x - x;
            blur.y = target.y - y;
        }
    }

    Rectangle {
        id: centerRectangle
        x: -50
        width: parent.width
        height: mainRectangle.height
        color: "#00000000"
        border.color: "#a1a1a1"
        border.width: 2

        MouseArea {
            anchors.fill: parent
        }

        Rectangle {
            id: rectangleMore
            width: 25
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            color: "#55a1a1a1"
            border.color: "#a1a1a1"
            border.width: 2
            radius: 5
            anchors.left: parent.right
            opacity: 0.25

            onVisibleChanged: {
                if(!visible)
                {
                    centerRectangle.x = -1 * mainRectangle.width;
                    rectangleMore.opacity = 0.2;
                }
            }

            Text {
                text: "\uf009"
                color: "#88000000"
                anchors.fill: parent
                font.pixelSize: 15
                font.family: mainIconFont.name
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea {
                anchors.fill: parent
                property int firstX
                property int firstTargetX

                onPressed: {
                    firstX = mouseX;
                    firstTargetX = centerRectangle.x;
                    animation1.stop();

                    animation2.to = 1;
                    animation2.start();
                }
                onMouseXChanged: {
                    centerRectangle.x = centerRectangle.x + mouseX - firstX;
                }
                onReleased: {
                    if(Math.abs(centerRectangle.x - firstTargetX) > 3)
                    {
                        if(centerRectangle.x > (mainRectangle.width * -0.3))
                        {
                            animation1.to = 0;
                            animation2.to = 1;
                        }
                        else
                        {
                            animation1.to = -1 * mainRectangle.width;
                            animation2.to = 0.2;
                        }
                    }
                    else
                    {
                        if(centerRectangle.x > (mainRectangle.width * -0.3))
                        {
                            animation1.to = -1 * mainRectangle.width;
                            animation2.to = 0.2;
                        }
                        else
                        {
                            animation1.to = 0;
                            animation2.to = 1;
                        }
                    }

                    animation1.restart();
                    animation2.restart();
                }
            }

            NumberAnimation{
                id: animation1
                duration: 500
                target: centerRectangle
                easing.type: Easing.OutQuad
                property: "x"
            }

            NumberAnimation{
                id: animation2
                duration: 200
                target: rectangleMore
                easing.type: Easing.OutQuad
                property: "opacity"
            }
        }

        Button {
            x: 0
            y: 5
            width: 50
            height: 50
            style: ButtonStyle {
                background: Text {
                    text: "\uf08b"
                    font.family: mainIconFont.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: ((width > height) ? (height) : (width)) - 12
                    color: (control.pressed) ? ("#303030") : ("#88303030")
                }
            }

            onClicked: {
                mainMessageBoxConfirm.show(qsTr("退出"), qsTr("确定要退出这个画室吗？"),
                                           function() {
                                               mainMessageBoxConfirm.hide();
                                               mainRectangle.onExitCallback();
                                           },
                                           mainMessageBoxConfirm.hide);
            }
        }

        Button {
            x: 0
            y: 101
            width: 50
            height: 50
            enabled: mainCanvas.edit
            style: ButtonStyle {
                background: Text {
                    text: "\uf0e2"
                    font.family: mainIconFont.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: ((width > height) ? (height) : (width)) - 12
                    color: (control.pressed) ? ("#303030") : ("#88303030")
                }
            }

            onClicked: {
                WhiteboardCanvasPainter.backToHistory();
                mainMessageBar.showMessage(qsTr("撤销操作"));
            }
        }

        Button {
            x: 0
            y: 53
            width: 50
            height: 50
            enabled: mainCanvas.edit
            style: ButtonStyle {
                background: Text {
                    text: "\uf01e"
                    font.family: mainIconFont.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: ((width > height) ? (height) : (width)) - 12
                    color: (control.pressed) ? ("#303030") : ("#88303030")
                }
            }

            onClicked: {
                WhiteboardCanvasPainter.backToFuture();
                mainMessageBar.showMessage(qsTr("恢复操作"));
            }
        }

        Button {
            x: 0
            y: 149
            width: 50
            height: 50
            enabled: mainCanvas.edit
            style: ButtonStyle {
                background: Text {
                    text: "\uf0c7"
                    font.family: mainIconFont.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: ((width > height) ? (height) : (width)) - 12
                    color: (control.pressed) ? ("#303030") : ("#88303030")
                }
            }

            onClicked: {
                WhiteboardCanvasPainter.save();
            }

            Connections{
                target: WhiteboardCanvasPainter
                onImageSaved: {
                    mainMessageBar.showMessage(qsTr("保存画板") + ((succeed) ? (qsTr("成功")) : (qsTr("失败"))));
                }
            }
        }

        Button {
            x: 0
            y: 197
            width: 50
            height: 50
            enabled: mainCanvas.edit
            style: ButtonStyle {
                background: Text {
                    text: "\uf07c"
                    font.family: mainIconFont.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: ((width > height) ? (height) : (width)) - 17
                    color: (control.pressed) ? ("#303030") : ("#88303030")
                }
            }

            onClicked: {
                WhiteboardCanvasPainter.open();
            }

            Connections{
                target: WhiteboardCanvasPainter
                onImageOpened: {
                    mainMessageBar.showMessage(qsTr("读取画板") + ((succeed) ? (qsTr("成功")) : (qsTr("失败"))));
                }
            }
        }

        ComboBox {
            id: comboBox
            x: 0
            y: 245
            width: 50
            height: 50
            model: ListModel {
                ListElement { text: "1"; }
                ListElement { text: "2"; }
                ListElement { text: "3"; }
            }
            style: ComboBoxStyle {
                background: Rectangle {
                    color: "#00000000"
                }
                label: Text {
                    font.family: mainIconFont.name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    text: comboBox.currentText
                    color: (comboBox.pressed) ? ("#000000") : ("#88000000")
                }
            }

            onCurrentIndexChanged: {
                WhiteboardCanvasPainter.setCurrentSourceIndex(currentIndex);
                if(count)
                {
                    mainMessageBar.showMessage(qsTr("切换至 ") + (currentIndex + 1));
                }
            }

            Connections {
                target: WhiteboardVop
                onCurrentBoardIndexChanged: {
                    comboBox.currentIndex = boardIndex;
                }
            }

            Text {
                anchors.fill: parent
                text: "\uf016"
                font.family: mainIconFont.name
                font.pixelSize: 33
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: (comboBox.pressed) ? ("#000000") : ("#88000000")
            }
        }
    }
}
