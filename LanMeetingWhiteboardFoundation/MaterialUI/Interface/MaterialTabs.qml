import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.2

Rectangle {
    id: tabs
    color: "#efefef"

    signal indexRefreshed();

    property var tabSource
    property int currentItemX
    property int currentItemWidth

    property alias currentIndex: tabView.currentIndex

    Component.onCompleted: {
        for(var index = 0; index < tabSource.length; index++)
        {
            tabView.addTab(tabSource[index]["title"], tabSource[index]["source"]);
        }
        indexRefreshed();
    }

    TabView {
        id: tabView
        anchors.fill: parent

        style: TabViewStyle {
            frameOverlap: 0
            tabOverlap: 0
            tabsAlignment: Qt.AlignHCenter

            tab: MaterialButton {
                implicitHeight: materialUI.dp(48)
                text: styleData.title
                textColor: (styleData.selected) ? ("#ffffff") : ("#a2ffffff")
                implicitWidth: Math.min(Math.max(labelWidth + 40, 80), tabView.width / tabView.count)

                function refreshPosition() {
                    if(tabView.currentIndex === styleData.index)
                    {
                        needle.x = parent.parent.x + parent.parent.parent.parent.x
                        needle.width = width
                    }
                }

                onClicked: {
                    tabView.currentIndex = styleData.index;
                }

                Connections {
                    target: parent.parent.parent.parent
                    onXChanged: {
                        refreshPosition();
                    }
                }

                Connections {
                    target: tabs

                    onWidthChanged: {
                        refreshPosition();
                    }

                    onCurrentIndexChanged: {
                        refreshPosition();
                    }

                    onIndexRefreshed: {
                        refreshPosition();
                    }
                }

                Behavior on textColor {
                    ColorAnimation { duration: 200; }
                }
            }

            frame: Rectangle {
                color: "#ffffff"
            }
        }

        Rectangle {
            x: 0
            y: -1 * materialUI.dp(48)
            z: -1
            width: tabView.width
            height: materialUI.dp(48)
            color: "#07bdd3"
        }
    }

    Rectangle {
        id: needle
        x: 0
        y: materialUI.dp(46)
        width: 20
        height: materialUI.dp(2)
        color: "#ffff95"

        Behavior on x {
            NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
        }

        Behavior on width {
            NumberAnimation { duration: 200; }
        }
    }
}
