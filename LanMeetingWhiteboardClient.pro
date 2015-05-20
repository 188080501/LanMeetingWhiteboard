TEMPLATE = app
TARGET = LanMeetingWhiteboard

QT += qml quick widgets network concurrent multimedia

include(../LanMeetingWhiteboardFoundation/Jason/Jdn.pri)
include(../LanMeetingWhiteboardFoundation/JasonLogCollection/JasonLogCollection.pri)
include(./MaterialUI/MaterialUI.pri)

SOURCES += main.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Foundation.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_QML.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Lan.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Net.cpp \
    Models/WhiteboardCanvas.cpp \
    Models/WhiteboardList.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_TcpClient.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_TcpServer.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_TcpSocket.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Container.cpp \
    Models/WhiteboardSocket.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Vop.cpp \
    Models/WhiteboardVop.cpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Trie.cpp

RESOURCES += qml.qrc \
    Resources/UI.qrc \
    iOSStyleWidgets/iOSStyleWidgets.qrc \
    iOSStyleWidgets/iOSStyleWidgetsResources.qrc \
    Resources/Vop.qrc \
    Resources/Font.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

mac{
    ICON = ./Resources/Icon/Icon.icns
}

win32{
    RC_FILE = ./Resources/Icon/Icon.rc
}

ios{
    HEADERS += \
     ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_iOS.h

    OBJECTIVE_SOURCES += \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_iOS.mm
}

INCLUDEPATH += ../LanMeetingWhiteboardFoundation/JasonQt \
    ../LanMeetingWhiteboardFoundation/LanMeetingWhiteboard

HEADERS += \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Foundation.h \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_QML.h \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Lan.h \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Net.h \
    Models/WhiteboardCanvas.h \
    Models/WhiteboardList.h \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_TcpClient.h \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_TcpServer.h \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_TcpSocket.h \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Container.h \
    Models/WhiteboardSocket.h \
    ../LanMeetingWhiteboardFoundation/LanMeetingWhiteboard/LanMeetingWhiteboardAgreement.hpp \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Vop.h \
    Models/WhiteboardVop.h \
    ../LanMeetingWhiteboardFoundation/JasonQt/JasonQt_Trie.h

DISTFILES += \
    Android/gradle/wrapper/gradle-wrapper.jar \
    Android/AndroidManifest.xml \
    Android/res/values/libs.xml \
    Android/build.gradle \
    Android/gradle/wrapper/gradle-wrapper.properties \
    Android/gradlew \
    Android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Android
