import QtQuick 2.0
import QtQuick.Controls 1.2 as Controls
import QtQuick.Controls.Styles 1.2 as ControlStyles
import "../Element"

Controls.Switch {
    id: control

    /*!
       The switch color. By default this is the app's accent color
     */
    property color color: darkBackground ? materialUI.dark.accentColor
                                         : materialUI.light.accentColor

    /*!
       Set to \c true if the switch is on a dark background
     */
    property bool darkBackground

    style: ControlStyles.SwitchStyle {
        handle: View {
            width: materialUI.dp(22)
            height: materialUI.dp(22)
            radius: height / 2
            elevation: 2
            backgroundColor: control.enabled ? control.checked ? control.color
                                                               : darkBackground ? "#BDBDBD"
                                                                                : "#FAFAFA"
                                             : darkBackground ? "#424242"
                                                              : "#BDBDBD"
        }

        groove: Item {
            width: materialUI.dp(40)
            height: materialUI.dp(22)

            Rectangle {
                anchors.centerIn: parent
                width: parent.width - materialUI.dp(2)
                height: materialUI.dp(16)
                radius: height / 2
                color: control.enabled ? control.checked ? materialUI.alpha(control.color, 0.5)
                                                         : darkBackground ? Qt.rgba(1, 1, 1, 0.26)
                                                                          : Qt.rgba(0, 0, 0, 0.26)
                                       : darkBackground ? Qt.rgba(1, 1, 1, 0.12)
                                                        : Qt.rgba(0, 0, 0, 0.12)

                Behavior on color {
                    ColorAnimation {
                        duration: 200
                    }
                }
            }
        }
    }
}
