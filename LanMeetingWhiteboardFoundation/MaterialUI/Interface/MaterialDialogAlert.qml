import QtQuick 2.0
import QtQuick.Controls 1.3 as Controls
import QtQuick.Controls.Styles 1.3 as Styles

MaterialDialog {
    id: dialog

    negativeButtonText: ""
    positiveButtonText: qsTr("OK")

    property var callbackOnOK: null

    function show(title, message, callbackOnOK) {
        dialog.title = title;
        dialog.text = message;
        dialog.callbackOnOK = callbackOnOK;
        dialog.open();

        materialUI.showBackground(function(){
            dialog.close();
            materialUI.hideBackground();

            if(dialog.callbackOnOK)
            {
                dialog.callbackOnOK();
            }
        });
    }

    onAccepted: {
        materialUI.hideBackground();

        if(callbackOnOK)
        {
            callbackOnOK();
        }
    }

    onRejected: {
        materialUI.hideBackground();

        if(callbackOnOK)
        {
            callbackOnOK();
        }
    }
}
