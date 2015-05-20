import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2

RadioButton {
    width: 30
    height: 30
    exclusiveGroup: targetExclusiveGroup
    clip: true

    property string radioText
    property var targetExclusiveGroup
    property color textColor: "#000000"

    style: RadioButtonStyle {
        indicator: Rectangle {
            implicitWidth: 30
            implicitHeight: 30
            radius: 15
            border.color: (control.checked) ? ((textColor != "#000000") ? (textColor) : ("gray")) : ("#00000000")
            border.width: 1
            color: "#00000000"

            Text {
                text: radioText
                font.pixelSize: 15
                anchors.fill: parent
                color: textColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
