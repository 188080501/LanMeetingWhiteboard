import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Rectangle {
    id: mainRectangle

    width: 270
    height: 250
    color: "#44000000"
    visible: false

    property bool userScale: true;

    property var needCallbackOnCancel
    property var needCallbackOnConfirm
    property var currentItemFlag
    property var currentItemText

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

        currentItemFlag = null;
        listModel.clear();
        for(var index = 0; index < listData.length; index++)
        {
            listModel.append({
                                 labelText: listData[index]["labelText"],
                                 defaultChecked: listData[index]["defaultChecked"],
                                 itemFlag: listData[index]["itemFlag"]
                             });
        }

        mainRectangle.visible = true;
        mainRectangle.focus = true;
        labelTitle.text = (titleText) ? (titleText) : ("Title");
        needCallbackOnCancel = (callbackOnCancel) ? (callbackOnCancel) : (null);
        needCallbackOnConfirm = (callbackOnConfirm) ? (callbackOnConfirm) : (null);
    }

    function hide(){
        closeAnimation.start();
    }

    function hideNow(){
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

    MouseArea {
        anchors.fill: parent
    }

    Keys.onEscapePressed: {
        if(buttonCancel.enabled)
        {
            buttonCancel.clicked();
        }
    }

    Keys.onEnterPressed: {
        if(buttonConfirm.enabled)
        {
            buttonConfirm.clicked();
        }
    }
    Keys.onReturnPressed: {
        if(buttonConfirm.enabled)
        {
            buttonConfirm.clicked();
        }
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
        id: showAnimation2;
        target: mainRectangle;
        property: "scale";
        from: 1.3;
        to: 1.0;
        duration: 300;
        easing.type: Easing.OutCubic
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


    Rectangle {
        id: centerRectangle
        width: 270
        height: 250
        color: "#00000000"
        radius: 1
        border.width: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        scale: (userScale) ? (JasonQtTool.calculateDoubleScale(mainRectangle.width, mainRectangle.height, 320, 568)) : (1.0);

        Image {
            anchors.fill: parent;
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/iOSStyleWidgetsResources/ChooseList.png"
        }

        Text {
            id: labelTitle
            x: 82
            y: 17
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 17
            font.bold: true
        }

        ExclusiveGroup{
            id: checkGroup
        }

        ListView {
            id: listView
            x: 0
            y: 55
            width: 270
            height: 150
            clip: true
            delegate: Item {
                width: 270
                height: 40

                Button {
                    anchors.fill: parent;
                    style: ButtonStyle {
                        background: Rectangle{
                            color: (control.pressed || radioButton.pressed) ? ("#30a4a4a4") : ("#00000000")
                        }
                    }

                    onClicked: {
                        radioButton.checked = true;
                        currentItemFlag = itemFlag;
                        currentItemText = labelText;
                    }
                }

                Rectangle {
                    x: 25
                    y: 39
                    width: 270
                    height: 1
                    color: "#50afafbc"
                }

                Text {
                    x: 30
                    y: 10
                    text: labelText
                    font.pixelSize: 17
                }

                RadioButton {
                    id: radioButton
                    x: 230
                    y: 10
                    width: 26
                    height: 21
                    exclusiveGroup: checkGroup
                    checked: defaultChecked

                    style: RadioButtonStyle {
                        indicator: Image {
                            width: 20
                            height: 16
                            fillMode: Image.Stretch
                            visible: control.checked
                            source: "qrc:/iOSStyleWidgetsResources/CheckIcon.png"
                        }
                    }

                    onClicked: {
                        currentItemFlag = itemFlag;
                        currentItemText = labelText;
                    }
                }

                Component.onCompleted: {
                    if(defaultChecked)
                    {
                        currentItemFlag = itemFlag;
                        currentItemText = labelText;
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
            width: parent.width / 2
            height: 45
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -1

            style: ButtonStyle {
                background: Rectangle {
                    color: (buttonCancel.pressed) ? ("#80e0e0e0") : ("#00000000")
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

        Button {
            id: buttonConfirm
            x: 135
            y: 136
            width: parent.width / 2
            height: 45
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -1

            style: ButtonStyle {
                background: Rectangle {
                    color: (buttonConfirm.pressed) ? ("#80e0e0e0") : ("#00000000")
                }

                label: Text {
                    text: qsTr("Confirm")
                    font.pixelSize: 17
                    color: (buttonConfirm.enabled) ? ("#157efb") : ("#8f8f8f")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            onClicked: {
                if(needCallbackOnConfirm)
                {
                    needCallbackOnConfirm(currentItemFlag);
                }
            }
        }
    }
}
