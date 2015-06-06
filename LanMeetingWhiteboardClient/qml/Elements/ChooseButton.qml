import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import "qrc:/MaterialUI/Element/"

RadioButton {
    id: radioButton
    width: 30
    height: 30
    clip: true

    property string text
    property color textColor: "#000000"
    property bool followColor: false

    style: RadioButtonStyle {
        indicator: Rectangle {
            id: background
            implicitWidth: 30
            implicitHeight: 30
            radius: 15
            border.color: "#00000000"
            border.width: 1
            color: "#00000000"

            Component.onCompleted: {
                if(!followColor && radioButton.checked)
                {
                    border.color = "#0091ff";
                }
                else if(checked)
                {
                    border.color = textColor;
                }
            }

            Connections {
                target: radioButton
                onTextColorChanged: {
                    if(control.checked)
                    {
                        border.color = textColor;
                    }
                    else
                    {
                        border.color = "#00000000";
                    }
                }
                onCheckedChanged: {
                    colorAnimation.to = (control.checked) ? ((radioButton.followColor) ? (textColor) : ("#0091ff")) : ("#00000000");
                    colorAnimation.restart();
                }
            }

            Text {
                id: mainText
                text: radioButton.text
                font.pixelSize: 15
                anchors.fill: parent
                color: radioButton.textColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            ColorAnimation {
                id: colorAnimation
                target: background
                properties: "border.color"
                duration: 400
                easing.type: Easing.InQuint
            }
        }
    }

    Ink {
        anchors.fill: parent

        width: materialUI.dp(40)
        height: materialUI.dp(40)
        color: radioButton.checked ? Qt.rgba(0,0,0,0.2) : Qt.rgba(0,0,0,0.1)
        onClicked: radioButton.checked = !radioButton.checked

        circular: true
        centered: true
    }
}
