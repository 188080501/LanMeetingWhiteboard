import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Rectangle {
    id: mainRectangle

    width: 42
    height: 42
    color: "#44000000"
    opacity: 0.0
    visible: false

    property bool userScale: true;

    property int currentIndex;

    function show() {
        showAnimation.start();
        showNow();
    }

    function showNow() {
        mainRectangle.visible = true;
        focus = true;
        currentIndex = 0;
        timerRefreshIndex.start();
    }

    function hide() {
        closeAnimation.from = mainRectangle.opacity;
        closeAnimation.start();
        timerRefreshIndex.stop();
    }

    function hideNow() {
        mainRectangle.visible = false;
        timerRefreshIndex.stop();
    }

    MouseArea {
        anchors.fill: parent
    }

    PropertyAnimation {
        id: showAnimation;
        target: mainRectangle;
        property: "opacity";
        from: 0.0;
        to: 1.0;
        duration: 200;
        easing.type: Easing.InCubic
    }

    PropertyAnimation {
        id: closeAnimation;
        target: mainRectangle;
        property: "opacity";
        to: 0.0;
        duration: 200;
        easing.type: Easing.OutCubic

        onStopped: {
            mainRectangle.visible = false;
        }
    }

    Timer {
        id: timerRefreshIndex;
        interval: 83;
        running: parent.visible
        repeat: true;

        onTriggered: {
            if(currentIndex >= 11)
            {
                currentIndex = 0;
            }
            else
            {
                currentIndex++;
            }
        }
    }

    Rectangle {
        id: centerRectangle
        width: 42;
        height: 42;
        color: "#00000000"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        scale: (userScale) ? (JasonQtTool.calculateDoubleScale(mainRectangle.width, mainRectangle.height, 320, 568)) : (1.0);

        Image {
            anchors.fill: parent;
            source: "qrc:/iOSStyleWidgetsResources/LoadingSpinner" + currentIndex + ".png"
        }
    }
}
