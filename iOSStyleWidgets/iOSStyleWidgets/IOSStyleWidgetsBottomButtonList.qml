import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Rectangle {
    id: mainRectangle;

    width: 270
    height: 333
    color: "#44000000"
    visible: false

    property bool userScale: true;

    property var needCallbackOnCancel
    property var needCallbackOnConfirm

    function show(titleText, listData, callbackOnCancel, callbackOnConfirm) {
        showAnimation.start();
        showAnimation2.start();
        showNow(titleText, listData, callbackOnCancel, callbackOnConfirm);
    }

    function showNow(titleText, listData, callbackOnCancel, callbackOnConfirm) {
        if(closeAnimation.running)
        {
            closeAnimation.stop();
        }

        listModel.clear();
        for(var index = 0; index < listData.length; index++)
        {
            listModel.append({
                                 labelText: listData[index]["labelText"],
                                 itemFlag: listData[index]["itemFlag"]
                             });
        }

        if(listData.length > 5)
        {
            listView.height = 225;
        }
        else
        {
            listView.height = listData.length * 45;
        }

        mainRectangle.visible = true;
        mainRectangle.focus = true;
        labelTitle.text = (titleText) ? (titleText) : ("Title");
        needCallbackOnCancel = (callbackOnCancel) ? (callbackOnCancel) : (null);
        needCallbackOnConfirm = (callbackOnConfirm) ? (callbackOnConfirm) : (null);
    }

    function hide() {
        closeAnimation.start();
        closeAnimation2.start();
    }

    function hideNow() {
        if(showAnimation.running)
        {
            showAnimation.stop();
        }
        if(showAnimation2.running)
        {
            showAnimation2.stop();
        }

        mainRectangle.visible = false;
    }

    Keys.onEscapePressed: {
        if(buttonCancel.enabled)
        {
            buttonCancel.clicked();
        }
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
        duration: 300;
        easing.type: Easing.InCubic
    }

    PropertyAnimation {
        id: showAnimation2;
        target: centerRectangle;
        property: "y";
        from: parent.height;
        to: parent.height - 8 - 333;
        duration: 400;
        easing.type: Easing.OutCubic
    }

    PropertyAnimation {
        id: closeAnimation;
        target: mainRectangle;
        property: "opacity";
        to: 0.0;
        duration: 500;
        easing.type: Easing.OutQuad

        onStopped: {
            mainRectangle.visible = false;
        }
    }

    PropertyAnimation {
        id: closeAnimation2;
        target: centerRectangle;
        property: "y";
        to: parent.height;
        duration: 500;
        easing.type: Easing.OutQuad
    }


    Rectangle {
        id: centerRectangle
        width: 270
        height: 333
        y: 0
        color: "#00000000"
        radius: 1
        border.width: 0
        anchors.horizontalCenter: parent.horizontalCenter
        scale: (userScale) ? (JasonQtTool.calculateDoubleScale(mainRectangle.width, mainRectangle.height, 320, 568)) : (1.0);

        Rectangle {
            y: labelTitle.y - 10
            width: listView.width
            height: listView.height + 40
            radius: 5
            color: "#ffffff"
        }

        Text {
            id: labelTitle
            x: 82
            color: "#8f8f8f"
            anchors.bottom: listView.top
            anchors.bottomMargin: 10
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 17
        }

        ListView {
            id: listView
            x: 0
            y: 55
            width: parent.width
            height: 225
            anchors.bottom: buttonCancel.top
            anchors.bottomMargin: 8
            clip: true
            delegate: Rectangle {
                width: 270
                height: 45
                color: "#00000000"

                Button {
                    anchors.fill: parent;
                    style: ButtonStyle {
                        background: Rectangle{
                            color: (control.pressed || button.pressed) ? ("#30a4a4a4") : ("#00000000")
                        }
                    }

                    onClicked: {
                        needCallbackOnConfirm(itemFlag);
                    }
                }

                Rectangle {
                    width: 270
                    height: 1
                    color: "#50afafbc"
                }

                Text {
                    anchors.fill: parent
                    text: labelText
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    color: "#157efb"
                }

                Button {
                    id: button
                    x: 230
                    y: 10
                    width: 26
                    height: 21
                    style: ButtonStyle {
                        background: Rectangle {
                            color: "#00000000"
                        }
                    }

                    onClicked: {
                        needCallbackOnConfirm(itemFlag);
                    }
                }
            }
            model: ListModel {
                id: listModel
            }
        }

        Button {
            id: buttonCancel
            y: 136
            width: parent.width
            height: 45
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom

            style: ButtonStyle {
                background: Rectangle {
                    radius: 5
                    color: (buttonCancel.pressed) ? ("#e0e0e0") : ("#ffffff")
                }

                label: Text {
                    text: qsTr("Cancel")
                    font.pixelSize: 17
                    color: "#157efb"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            onClicked: {
                if(needCallbackOnCancel)
                {
                    needCallbackOnCancel();
                }
            }
        }
    }
}
