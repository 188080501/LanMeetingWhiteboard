import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2

Rectangle {
    id: mainRectangle
    width: 100
    height: 40
    color: "#99222222"
    opacity: 0

    function showMessage(message) {
        labelText.text = message;
        closeAnimation.restart();
    }

    PropertyAnimation {
        id: closeAnimation
        target: mainRectangle
        property: "opacity"
        duration: 1500
        from: 1
        to: 0
        easing.type: Easing.InQuart
    }

    Text {
        id: labelText
        font.pixelSize: 16
        color: "#ffffff"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.fill: parent
    }
}
