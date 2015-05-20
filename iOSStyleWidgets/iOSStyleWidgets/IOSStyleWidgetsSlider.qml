import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Slider {
    id: slider
    width: 200
    height: 43

    property color grooveColor: "#157efb"

    style: SliderStyle {
        groove: Rectangle {
            color: "#00000000"

            Rectangle {
                width: parent.width - 30
                height: 2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: slider.height / -10
                radius: 4
                color: grooveColor
            }
        }
        handle: Rectangle {
            implicitWidth: 48
            implicitHeight: 48
            color: "#00000000";
            Image {
                anchors.fill: parent;
                fillMode: Image.PreserveAspectCrop
                source: "qrc:/iOSStyleWidgetsResources/SliderToggle.png";
            }
        }
    }
}
