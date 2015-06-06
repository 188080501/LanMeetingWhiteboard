import QtQuick 2.0
import QtQuick.Controls 1.3

MaterialDialog {
    id: dialog

    negativeButtonText: qsTr("Cancel")
    positiveButtonText: qsTr("OK")

    property var callbackOnCancel: null
    property var callbackOnOK: null

    property int currentItemIndex
    property string currentItemText
    property var currentItemFlag

    function show(title, message, listData, callbackOnCancel, callbackOnOK) {
        dialog.title = title;
        dialog.text = message;
        dialog.callbackOnCancel = callbackOnCancel;
        dialog.callbackOnOK = callbackOnOK;
        dialog.open();

        currentItemIndex = -1;
        currentItemText = "";
        currentItemFlag = null;

        materialUI.showBackground();

        listModel.clear();
        for(var index = 0; index < listData.length; index++)
        {
            listModel.append({
                                 itemIndex: index,
                                 itemText: listData[index]["text"],
                                 itemFlag: ("flag" in listData[index]) ? (listData[index]["flag"]) : (null),
                                 defaultChecked: ("checked" in listData[index]) ? (listData[index]["checked"]) : (false)
                             });
        }
    }

    onAccepted: {
        materialUI.hideBackground();

        if(callbackOnOK)
        {
            callbackOnOK(currentItemIndex, currentItemText, currentItemFlag);
        }
    }

    onRejected: {
        materialUI.hideBackground();

        if(callbackOnCancel)
        {
            callbackOnCancel();
        }
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
            height: 50

            MaterialButton {
                anchors.fill: parent

                onClicked: {
                    radioButton.checked = true;
                    currentItemIndex = itemIndex;
                    currentItemText = itemText;
                    currentItemFlag = itemFlag;
                }
            }

            Rectangle {
                x: 10
                y: 49
                width: 270
                height: 1
                color: "#50afafbc"
            }

            Text {
                x: 50
                y: 15
                text: itemText
                font.pixelSize: 17
            }

            MaterialRadioButton {
                id: radioButton
                y: 15
                width: 25
                height: 20
                exclusiveGroup: checkGroup
                checked: defaultChecked

                onClicked: {
                    currentItemIndex = itemIndex;
                    currentItemText = itemText;
                    currentItemFlag = itemFlag;
                }
            }

            Component.onCompleted: {
                if(defaultChecked)
                {
                    currentItemIndex = itemIndex;
                    currentItemText = itemText;
                    currentItemFlag = itemFlag;
                }
            }
        }
        model: ListModel {
            id: listModel
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#e7e7e7"
            anchors.left: listView.left
            anchors.bottom: listView.bottom
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#e7e7e7"
            anchors.left: listView.left
            anchors.top: listView.top
        }
    }
}
