// Qt lib import
#include <QApplication>
#include <QQmlApplicationEngine>

// JasonQt lib import
#include "JasonQt_QML.h"
#include "JasonQt_Settings.h"

// LanMeetingWhiteboard lib import
#include "Models/WhiteboardList.h"
#include "Models/WhiteboardCanvas.h"
#include "Models/WhiteboardSocket.h"
#include "Models/WhiteboardVop.h"

// JasonLogCollection lib import
#include "JasonLogCollectionApi.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JasonLogCollectionApi::standardHandleProcess("LanMeetingWhiteboard", "GitHub", "0edba7917033b643d395748ecf91adb1f6a80929", "RunKey", 0);

    JasonQt_QML::Tool jasonQtTool;
    WhiteboardList whiteboardList;
    WhiteboardCanvasSource *whiteboardCanvasSource = new WhiteboardCanvasSource;
    WhiteboardCanvasPainter whiteboardCanvasPainter(whiteboardCanvasSource);
    WhiteboardSocket whiteboardSocket(&whiteboardList, &whiteboardCanvasPainter);
    WhiteboardVop whiteboardVop(&whiteboardCanvasPainter);
    JasonQt_Settings::Set whiteboardSettings("Data", "Settings", "LanMeetingWhiteboard");

    whiteboardCanvasPainter.setSocket(&whiteboardSocket);

    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<WhiteboardCanvasPainter>("LanMeetingWhiteboard", 1, 0, "PaintGroup", "This exports PaintGroup enums to QML");

    engine.rootContext()->setContextProperty("JasonQtTool", &jasonQtTool);
    engine.rootContext()->setContextProperty("WhiteboardList", &whiteboardList);
    engine.addImageProvider("WhiteboardCanvasSource", whiteboardCanvasSource);
    engine.rootContext()->setContextProperty("WhiteboardCanvasPainter", &whiteboardCanvasPainter);
    engine.rootContext()->setContextProperty("WhiteboardSocket", &whiteboardSocket);
    engine.rootContext()->setContextProperty("WhiteboardVop", &whiteboardVop);
    engine.rootContext()->setContextProperty("WhiteSettings", &whiteboardSettings);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
