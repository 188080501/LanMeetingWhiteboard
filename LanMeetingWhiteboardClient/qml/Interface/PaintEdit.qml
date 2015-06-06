import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import LanMeetingWhiteboard 1.0
import QtGraphicalEffects 1.0
import "qrc:/qml/Elements/"

Rectangle {
    id: mainRectangle
    width: 270
    height: 100
    color: "#00000000"

    property int currentPaintGroup: PaintGroup.PaintLine

    function hide() {
        if(centerRectangle.y !== -1 * mainRectangle.height)
        {
            animation1.to = -1 * mainRectangle.height;
            animation2.to = 0.2;

            animation1.restart();
            animation2.restart();
        }
    }

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
        x: 0
        y: -100
        width: mainRectangle.width
        height: parent.height
        color: "#00000000"
        border.color: "#a1a1a1"
        border.width: 2

        MouseArea {
            anchors.fill: parent
        }

        Rectangle {
            id: rectangleMore
            width: 50
            height: 25
            color: "#55a1a1a1"
            border.color: "#a1a1a1"
            border.width: 2
            radius: 5
            anchors.top: parent.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 0.25

            onVisibleChanged: {
                if(!visible)
                {
                    hide();
                }
            }

            Text {
                text: "\uf1fc"
                color: "#88000000"
                anchors.fill: parent
                font.pixelSize: 15
                font.family: mainIconFont.name
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea {
                anchors.fill: parent
                property int firstY
                property int firstTargetY

                onPressed: {
                    firstY = mouseY;
                    firstTargetY = centerRectangle.y
                    animation1.stop();

                    animation2.to = 1;
                    animation2.start();
                }
                onMouseYChanged: {
                    centerRectangle.y = centerRectangle.y + mouseY - firstY;
                }
                onReleased: {
                    if(Math.abs(centerRectangle.y - firstTargetY) > 3)
                    {
                        if(centerRectangle.y > (mainRectangle.height * -0.3))
                        {
                            animation1.to = 0;
                            animation2.to = 1;
                        }
                        else
                        {
                            animation1.to = -1 * mainRectangle.height;
                            animation2.to = 0.2;
                        }
                    }
                    else
                    {
                        if(centerRectangle.y > (mainRectangle.height * -0.3))
                        {
                            animation1.to = -1 * mainRectangle.height;
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
                property: "y"
            }

            NumberAnimation{
                id: animation2
                duration: 200
                target: rectangleMore
                easing.type: Easing.OutQuad
                property: "opacity"
            }
        }

        ExclusiveGroup {
            id: paintExclusiveGroup
        }

        ChooseButton {
            x: 6
            y: 57
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("点")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintPoint;
                    materialUI.showSnackbarMessage(qsTr("绘制点"));
                }
            }
        }

        ChooseButton {
            x: 41
            y: 57
            checked: true
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("线")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintLine;
                    materialUI.showSnackbarMessage(qsTr("绘制线"));
                }
            }
        }

        ChooseButton {
            x: 77
            y: 57
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("方")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintRectangle;
                    materialUI.showSnackbarMessage(qsTr("绘制方块"));
                }
            }
        }

        ChooseButton {
            x: 113
            y: 57
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("圆")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintEllipse;
                    materialUI.showSnackbarMessage(qsTr("绘制圆形"));
                }
            }
        }

        ChooseButton {
            x: 149
            y: 57
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("箭")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintArrow;
                    materialUI.showSnackbarMessage(qsTr("绘制箭头"));
                }
            }
        }

        ChooseButton {
            x: 184
            y: 57
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("擦")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintEraser;
                    materialUI.showSnackbarMessage(qsTr("选择了橡皮擦"));
                }
            }
        }

        ChooseButton {
            x: 6
            y: 13
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("星")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintStar;
                    materialUI.showSnackbarMessage(qsTr("绘制星星"));
                }
            }
        }

        ChooseButton {
            x: 41
            y: 13
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("心")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintHeart;
                    materialUI.showSnackbarMessage(qsTr("绘制爱心"));
                }
            }
        }

        ChooseButton {
            x: 113
            y: 13
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("字")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintText;
                    materialUI.showSnackbarMessage(qsTr("绘制文字"));
                }
            }
        }

        ChooseButton {
            x: 149
            y: 13
            exclusiveGroup: paintExclusiveGroup
            text: qsTr("图")

            onCheckedChanged: {
                if(checked)
                {
                    currentPaintGroup = PaintGroup.PaintImage;
                    materialUI.showSnackbarMessage(qsTr("绘制图片"));
                    WhiteboardCanvasPainter.readyDrawImage();
                }
            }
        }

        Button {
            x: 229
            y: 35
            width: 30
            height: 30
            style: ButtonStyle {
                background: Rectangle {
                    color: "#00000000"
                }
                label: Text {
                    text: qsTr("清");
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 15
                }
            }

            onClicked: {
                WhiteboardCanvasPainter.drawBegin();
                WhiteboardCanvasPainter.drawClear();
                WhiteboardCanvasPainter.drawEnd();
                materialUI.showSnackbarMessage(qsTr("清空画板"));
            }
        }

        Rectangle {
            id: rectangle1
            x: 222
            y: 7
            width: 1
            height: 86
            color: "#a1a1a1"
            border.width: 0
        }
    }
}
