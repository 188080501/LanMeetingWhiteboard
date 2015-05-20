import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import LanMeetingWhiteboard 1.0

Rectangle {
    id: mainRectangle
    width: 320
    height: 548
    clip: true

    property var onExitCallback
    property bool edit: false

    function show() {
        visible = true;
        openOrCloseAnimation.from = mainRectangle.height;
        openOrCloseAnimation.to = 0;
        openOrCloseAnimation.start();
    }

    function hide() {
        openOrCloseAnimation.from = 0;
        openOrCloseAnimation.to = mainRectangle.height;
        openOrCloseAnimation.start();
    }

    PropertyAnimation {
        id: openOrCloseAnimation
        target: mainRectangle
        property: "y"
        duration: 500
        easing.type: Easing.OutCubic

        onStopped: {
            if(to !== 0)
            {
                mainRectangle.visible = false;
            }
        }
    }

    Image {
        id: imageSource
        anchors.fill: parent
        source: "image://WhiteboardCanvasSource/";

        Connections {
            target: WhiteboardCanvasPainter

            property int count: 0

            onImageChanged: {
                imageSource.source = "image://WhiteboardCanvasSource/source/" + count++;
            }
        }
    }

    Image {
        id: imageCover
        anchors.fill: parent
        source: "image://WhiteboardCanvasSource/";

        Connections {
            target: WhiteboardCanvasPainter

            property int count: 0

            onCoverChanged: {
                imageCover.source = "image://WhiteboardCanvasSource/cover/" + count++;
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        property int bufX
        property int bufY
        property int currentX
        property int currentY

        onPressed: {
            if(!edit)
            {
                imageDisabled.visible = true;
                imageDisabled.x = mouseX - 15;
                imageDisabled.y = mouseY - 15;
                return;
            }

            currentX = mouseX / mainRectangle.width * 640;
            currentY = mouseY / mainRectangle.height * 1096;

            switch(paintEdit.currentPaintGroup)
            {
                case PaintGroup.PaintPoint:
                    WhiteboardCanvasPainter.drawBegin();
                    WhiteboardCanvasPainter.drawPoint(currentX, currentY);
                    WhiteboardCanvasPainter.drawEnd();
                    break;
                case PaintGroup.PaintLine:
                    WhiteboardCanvasPainter.drawBegin();
                    WhiteboardCanvasPainter.drawLinePoint(currentX, currentY);
                    bufX = currentX;
                    bufY = currentY;
                    break;
                case PaintGroup.PaintRectangle:
                case PaintGroup.PaintEllipse:
                case PaintGroup.PaintArrow:

                case PaintGroup.PaintStar:
                case PaintGroup.PaintHeart:
                    WhiteboardCanvasPainter.drawBegin();
                    bufX = currentX;
                    bufY = currentY;
                    break;

                case PaintGroup.PaintText:
                    mainMessageBoxPrompt.show(qsTr("绘画文字"), qsTr("请输入要绘画的文字"), qsTr("文字"), "",
                                              mainMessageBoxPrompt.hide,
                                              function(text){
                                                  mainMessageBoxPrompt.hide();
                                                  WhiteboardCanvasPainter.drawBegin();
                                                  WhiteboardCanvasPainter.drawText(currentX, currentY, text);
                                                  WhiteboardCanvasPainter.drawEnd();
                                              })
                    break;
                case PaintGroup.PaintImage:
                    WhiteboardCanvasPainter.drawBegin();
                    bufX = currentX;
                    bufY = currentY;
                    break;

                case PaintGroup.PaintEraser:
                    WhiteboardCanvasPainter.drawBegin();
                    WhiteboardCanvasPainter.drawEraserPoint(currentX, currentY);
                    bufX = currentX;
                    bufY = currentY;
                    break;
            }
        }
        onPositionChanged: {
            if(!edit)
            {
                imageDisabled.visible = true;
                imageDisabled.x = mouseX - 15;
                imageDisabled.y = mouseY - 15;
                return;
            }

            currentX = mouseX / mainRectangle.width * 640;
            currentY = mouseY / mainRectangle.height * 1096;

            switch(paintEdit.currentPaintGroup)
            {
                case PaintGroup.PaintLine:
                    WhiteboardCanvasPainter.drawLine(bufX, bufY, currentX, currentY);
                    bufX = currentX;
                    bufY = currentY;
                    break;
                case PaintGroup.PaintRectangle:
                    WhiteboardCanvasPainter.clearCover();
                    WhiteboardCanvasPainter.drawRectangle(bufX, bufY, currentX, currentY);
                    break;
                case PaintGroup.PaintEllipse:
                    WhiteboardCanvasPainter.clearCover();
                    WhiteboardCanvasPainter.drawEllipse(bufX, bufY, currentX, currentY);
                    break;
                case PaintGroup.PaintArrow:
                    WhiteboardCanvasPainter.clearCover();
                    WhiteboardCanvasPainter.drawArrow(bufX, bufY, currentX, currentY);
                    break;

                case PaintGroup.PaintStar:
                    WhiteboardCanvasPainter.clearCover();
                    WhiteboardCanvasPainter.drawStar(bufX, bufY, currentX, currentY);
                    break;
                case PaintGroup.PaintHeart:
                    WhiteboardCanvasPainter.clearCover();
                    WhiteboardCanvasPainter.drawHeart(bufX, bufY, currentX, currentY);
                    break;

                case PaintGroup.PaintImage:
                    WhiteboardCanvasPainter.clearCover();
                    WhiteboardCanvasPainter.drawImage(bufX, bufY, currentX, currentY);
                    break;

                case PaintGroup.PaintEraser:
                    WhiteboardCanvasPainter.drawEraserLine(bufX, bufY, currentX, currentY);
                    bufX = currentX;
                    bufY = currentY;
                    break;
            }
        }
        onReleased: {
            if(!edit)
            {
                imageDisabled.visible = false;
                return;
            }

            switch(paintEdit.currentPaintGroup)
            {
                case PaintGroup.PaintLine:
                    WhiteboardCanvasPainter.drawLineEnd();
                    WhiteboardCanvasPainter.drawEnd();
                    break;
                case PaintGroup.PaintRectangle:
                case PaintGroup.PaintEllipse:
                case PaintGroup.PaintArrow:
                case PaintGroup.PaintStar:
                case PaintGroup.PaintHeart:
                case PaintGroup.PaintImage:
                    WhiteboardCanvasPainter.drawEnd();
                    break;
                case PaintGroup.PaintEraser:
                    WhiteboardCanvasPainter.drawEraserEnd();
                    WhiteboardCanvasPainter.drawEnd();
                    break;
            }
        }
    }

    BorderImage {
        id: imageDisabled
        x: 117
        y: 178
        width: 30
        height: 30
        visible: !edit
        source: "qrc:/UI/Disabled.png"
    }

    OtherEdit {
        id: otherEdit
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        onExitCallback: mainRectangle.onExitCallback
    }

    PaintEdit {
        id: paintEdit
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }

    PenEdit {
        id: penEdit
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
    }

    CheckBox {
        width: 30
        height: 30
        anchors.bottomMargin: 3
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        style: CheckBoxStyle {
            indicator: Text {
                width: 30
                height: 30
                text: "\uf130"
                color: (control.checked) ? ("#000000") : ("#88000000")
                font.family: mainIconFont.name
                font.pixelSize: (width > height) ? (height) : (width)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }

        onClicked: {
            if(checked)
            {
                WhiteboardVop.start();
            }
            else
            {
                WhiteboardVop.finish();
            }
        }

        Connections {
            target: WhiteboardVop
            onMessageEmit: {
                mainMessageBar.showMessage(message);
            }
        }
    }
}
