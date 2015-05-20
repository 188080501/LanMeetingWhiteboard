import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import "qrc:/qml/Elements/"
import "qrc:/MaterialUI/Interface/"

Rectangle {
    id: mainRectangle
    width: 200
    height: 300
    color: "#00000000"

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
        x: 200
        width: 200
        height: parent.height
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
            anchors.right: parent.left
            anchors.verticalCenter: parent.verticalCenter
            color: "#55a1a1a1"
            border.color: "#a1a1a1"
            border.width: 2
            radius: 5
            opacity: 0.25

            onVisibleChanged: {
                if(!visible)
                {
                    centerRectangle.x = mainRectangle.width;
                    rectangleMore.opacity = 0.2;
                }
            }

            Text {
                text: "\uf040"
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
                        if(centerRectangle.x < (mainRectangle.width * 0.3))
                        {
                            animation1.to = 0;
                            animation2.to = 1;
                        }
                        else
                        {
                            animation1.to = mainRectangle.width;
                            animation2.to = 0.2;
                        }
                    }
                    else
                    {
                        if(centerRectangle.x < (mainRectangle.width * 0.3))
                        {
                            animation1.to = mainRectangle.width;
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

        ExclusiveGroup {
            id: penExclusiveGroup
        }

        Connections {
            target: WhiteboardVop
            onPenColorChanged: {
                chooseButtonDefined.checked = true;
                sliderHorizontalR.value = penColor.r;
                sliderHorizontalG.value = penColor.g;
                sliderHorizontalB.value = penColor.b;
            }
        }

        ChooseButton {
            x: 13
            y: 10
            checked: true
            targetExclusiveGroup: penExclusiveGroup
            radioText: qsTr("黑")

            onClicked: {
                sliderHorizontalR.value = 0;
                sliderHorizontalG.value = 0;
                sliderHorizontalB.value = 0;
                checked = true;
                WhiteboardCanvasPainter.setPenColor(Qt.rgba(0, 0, 0, 255));
                chooseButtonDefined.textColor = Qt.rgba(0, 0, 0, 255);
                mainMessageBar.showMessage(qsTr("选择了黑色"));
            }
        }

        ChooseButton {
            x: 49
            y: 10
            targetExclusiveGroup: penExclusiveGroup
            radioText: qsTr("红")
            textColor: "#ff0000"

            onClicked: {
                sliderHorizontalR.value = 255;
                sliderHorizontalG.value = 0;
                sliderHorizontalB.value = 0;
                checked = true;
                WhiteboardCanvasPainter.setPenColor(Qt.rgba(255, 0, 0, 255));
                chooseButtonDefined.textColor = Qt.rgba(0, 0, 0, 255);
                mainMessageBar.showMessage(qsTr("选择了红色"));
            }
        }

        ChooseButton {
            x: 85
            y: 10
            targetExclusiveGroup: penExclusiveGroup
            radioText: qsTr("绿")
            textColor: "#00ff00"

            onClicked: {
                sliderHorizontalR.value = 0;
                sliderHorizontalG.value = 255;
                sliderHorizontalB.value = 0;
                checked = true;
                WhiteboardCanvasPainter.setPenColor(Qt.rgba(0, 255, 0, 255));
                chooseButtonDefined.textColor = Qt.rgba(0, 0, 0, 255);
                mainMessageBar.showMessage(qsTr("选择了绿色"));
            }
        }

        ChooseButton {
            x: 121
            y: 10
            targetExclusiveGroup: penExclusiveGroup
            radioText: qsTr("蓝")
            textColor: "#0000ff"

            onClicked: {
                sliderHorizontalR.value = 0;
                sliderHorizontalG.value = 0;
                sliderHorizontalB.value = 255;
                checked = true;
                WhiteboardCanvasPainter.setPenColor(Qt.rgba(0, 0, 255, 255));
                chooseButtonDefined.textColor = Qt.rgba(0, 0, 0, 255);
                mainMessageBar.showMessage(qsTr("选择了蓝色"));
            }
        }

        ChooseButton {
            id: chooseButtonDefined
            x: 157
            y: 10
            targetExclusiveGroup: penExclusiveGroup
            radioText: qsTr("定")

            onClicked: {
                chooseButtonDefined.textColor = Qt.rgba(sliderHorizontalR.value, sliderHorizontalG.value, sliderHorizontalB.value, 255);
                mainMessageBar.showMessage(qsTr("自定义颜色"));
            }
        }

        MaterialSlider {
            id: sliderHorizontalR
            x: 53
            y: 52
            width: 130
            height: 22
            color: "#000000"

            onValueChanged: {
                color.r = value;
                color.g = 0;
                color.b = 0;

                WhiteboardCanvasPainter.setPenColor(Qt.rgba(sliderHorizontalR.value, sliderHorizontalG.value, sliderHorizontalB.value, 1.0));
                chooseButtonDefined.textColor = Qt.rgba(sliderHorizontalR.value, sliderHorizontalG.value, sliderHorizontalB.value, 255);
                chooseButtonDefined.checked = true;
            }
        }

        MaterialSlider {
            id: sliderHorizontalG
            x: 53
            y: 85
            width: 130
            height: 22
            color: "#000000"

            onValueChanged: {
                color.r = 0;
                color.g = value;
                color.b = 0;

                WhiteboardCanvasPainter.setPenColor(Qt.rgba(sliderHorizontalR.value, sliderHorizontalG.value, sliderHorizontalB.value, 1.0));
                chooseButtonDefined.textColor = Qt.rgba(sliderHorizontalR.value, sliderHorizontalG.value, sliderHorizontalB.value, 255);
                chooseButtonDefined.checked = true;
            }
        }

        MaterialSlider {
            id: sliderHorizontalB
            x: 53
            y: 119
            width: 130
            height: 22
            color: "#000000"

            onValueChanged: {
                color.r = 0;
                color.g = 0;
                color.b = value;

                WhiteboardCanvasPainter.setPenColor(Qt.rgba(sliderHorizontalR.value, sliderHorizontalG.value, sliderHorizontalB.value, 1.0));
                chooseButtonDefined.textColor = Qt.rgba(sliderHorizontalR.value, sliderHorizontalG.value, sliderHorizontalB.value, 255);
                chooseButtonDefined.checked = true;
            }
        }

        Text {
            x: 18
            y: 49
            width: 30
            height: 22
            text: qsTr("红：")
            color: sliderHorizontalR.color
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 15
        }

        Text {
            x: 18
            y: 82
            width: 30
            height: 22
            text: qsTr("绿：")
            color: sliderHorizontalG.color
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            x: 18
            y: 116
            width: 30
            height: 22
            text: qsTr("蓝：")
            color: sliderHorizontalB.color
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
            id: rectangle1
            x: 10
            y: 151
            width: 180
            height: 1
            color: "#a1a1a1"
        }

        Rectangle {
            id: rectangleWidthPreview
            x: 95
            y: 160
            width: 13
            height: 22
            color: "#000000"
            visible: false
            anchors.horizontalCenterOffset: 63
            anchors.horizontalCenter: sliderHorizontalWidth.horizontalCenter
        }

        MaterialSlider {
            id: sliderHorizontalWidth
            x: 16
            y: 196
            width: 168
            height: 22
            maximumValue: 20
            color: "#a1a1a1"

            onValueChanged: {
                var currentValue = parseInt(value), targetValue;

                if(currentValue > 1)
                {
                    switch(currentValue)
                    {
                        case 2: targetValue = 1; break;
                        case 3: targetValue = 1; break;
                        case 4: targetValue = 2; break;
                        case 5: targetValue = 2; break;
                        case 6: targetValue = 3; break;
                        case 7: targetValue = 4; break;
                        case 8: targetValue = 5; break;
                        case 9: targetValue = 6; break;
                        case 10: targetValue = 8; break;
                        case 11: targetValue = 10; break;
                        case 12: targetValue = 12; break;
                        case 13: targetValue = 14; break;
                        case 14: targetValue = 16; break;
                        case 15: targetValue = 18; break;
                        case 16: targetValue = 20; break;
                        case 17: targetValue = 25; break;
                        case 18: targetValue = 30; break;
                        case 19: targetValue = 35; break;
                        case 20: targetValue = 40; break;
                    }

                    WhiteboardCanvasPainter.setPenWidth(targetValue);
                    labelWidth.text = targetValue + "px";
                    rectangleWidthPreview.visible = true;
                    rectangleWidthPreview.width = (targetValue === 1) ? (1) : (targetValue / 2);
                }
                else
                {
                    WhiteboardCanvasPainter.setPenWidth(-1);
                    labelWidth.text = qsTr("自动")
                    rectangleWidthPreview.visible = false;
                }
            }
        }

        Text {
            x: 18
            y: 160
            width: 30
            height: 22
            text: qsTr("宽：")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 15
        }

        Text {
            id: labelWidth
            x: 54
            y: 160
            width: 133
            height: 22
            text: qsTr("自动")
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 15
        }

        Rectangle {
            id: rectangle2
            x: 10
            y: 225
            width: 180
            height: 1
            color: "#a1a1a1"
        }

        Connections {
            target: WhiteboardVop
            onPenOpacityChanged: {
                sliderHorizontalOpacity.value = penOpacity;
            }
        }

        Image {
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            source: "qrc:/UI/Opacity.png"
            anchors.fill: rectangleOpacityPreview
        }

        Rectangle {
            id: rectangleOpacityPreview
            x: 95
            y: 235
            width: 22
            height: 22
            color: "#000000"
            anchors.horizontalCenterOffset: 66
            anchors.horizontalCenter: sliderHorizontalOpacity.horizontalCenter
        }

        MaterialSlider {
            id: sliderHorizontalOpacity
            x: 16
            y: 269
            width: 168
            height: 22
            maximumValue: 100
            color: "#a1a1a1"

            Component.onCompleted: {
                value = 100;
            }

            onValueChanged: {
                var currentValue = parseInt(value);

                if(currentValue > 99)
                {
                    WhiteboardCanvasPainter.setPenOpacity(1);
                    labelOpacity.text = qsTr("不透明")
                    rectangleOpacityPreview.opacity = 1;
                }
                else if(currentValue < 1)
                {
                    WhiteboardCanvasPainter.setPenOpacity(0);
                    labelOpacity.text = qsTr("完全透明")
                    rectangleOpacityPreview.opacity = 0;
                }
                else
                {
                    WhiteboardCanvasPainter.setPenOpacity(currentValue / 100);
                    labelOpacity.text = currentValue + "%";
                    rectangleOpacityPreview.opacity = currentValue / 100;
                }
            }
        }

        Text {
            x: 49
            y: 235
            width: 30
            height: 22
            text: qsTr("透明度：")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 15
        }

        Text {
            id: labelOpacity
            x: 85
            y: 235
            width: 50
            height: 22
            text: qsTr("不透明")
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 15
        }
    }
}
