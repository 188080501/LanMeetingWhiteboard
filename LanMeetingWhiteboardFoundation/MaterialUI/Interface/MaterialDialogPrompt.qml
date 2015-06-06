import QtQuick 2.0
import QtQuick.Controls 1.3 as Controls
import QtQuick.Controls.Styles 1.3 as Styles

MaterialDialog {
    id: dialog

    negativeButtonText: qsTr("Cancel")
    positiveButtonText: qsTr("OK")

    property var callbackOnCancel: null
    property var callbackOnOK: null

    function show(title, message, placeholderText, currentText, callbackOnCancel, callbackOnOK) {
        dialog.title = title;
        dialog.text = message;
        dialog.callbackOnCancel = callbackOnCancel;
        dialog.callbackOnOK = callbackOnOK;
        dialog.open();

        textField.placeholderText = placeholderText;
        textField.text = currentText;

        materialUI.showBackground();
    }

    onAccepted: {
        materialUI.hideBackground();

        if(callbackOnOK)
        {
            callbackOnOK(textField.text);
        }
    }

    onRejected: {
        materialUI.hideBackground();

        if(callbackOnCancel)
        {
            callbackOnCancel();
        }
    }

    MaterialTextField {
        id: textField
        width: parent.width
    }
}
