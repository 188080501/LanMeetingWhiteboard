#ifndef __WhiteboardCanvas_h__
#define __WhiteboardCanvas_h__

// Qt lib import
#include <QQuickImageProvider>
#include <QFileDialog>

// JasonQt lib import
#include "JasonQt_Foundation.h"
#ifdef Q_OS_IOS
#   include "JasonQt_iOS.h"
#endif

// LanMeetingWhiteboard lib import
#include "LanMeetingWhiteboardAgreement.hpp"

class WhiteboardCanvasPainter;
class WhiteboardSocket;

class WhiteboardCanvasSource: public QQuickImageProvider
{
    friend class WhiteboardCanvasPainter;

private:
    struct SourceNode
    {
        QVector<QByteArray> history;
        int historyIndex = 0;

        QImage current;
        QImage cover;
    };

private:
    std::array<SourceNode, WhiteboardMaxSourceSize> m_source;
    int m_currentSourceIndex = 0;

public:
    WhiteboardCanvasSource(void);

    QImage requestImage(const QString &id, QSize *, const QSize &);
};

class WhiteboardCanvasPainter: public QObject
{
    Q_OBJECT

public:
    Q_ENUMS(PaintGroupEnum)

    enum PaintGroupEnum
    {
        UnKnowPaint                             = Agreement::UnKnowPaint,
        PaintPoint                              = Agreement::PaintPoint,
        PaintLine                               = Agreement::PaintLine,
        PaintRectangle                          = Agreement::PaintRectangle,
        PaintEllipse                            = Agreement::PaintEllipse,
        PaintArrow                              = Agreement::PaintArrow,

        PaintStar                               = Agreement::PaintStar,
        PaintHeart                              = Agreement::PaintHeart,

        PaintText                               = Agreement::PaintText,
        PaintImage                              = Agreement::PaintImage,

        PaintEraser                             = Agreement::PaintEraser
    };

private:
    WhiteboardCanvasSource *m_source;
    WhiteboardSocket *m_socket;
    QMutex m_mutex;

    QColor m_penColor;
    int m_penWidth = -1;
    double m_penOpacity = 1.0;

    QImage m_drawImageData;

    int m_imagePickerCurrentAction = 0;
#ifdef Q_OS_IOS
    JasonQt_iOS::ImagePicker m_imagePicker;
#endif

    QVector< std::pair<int, QPainterPath > > m_line;
    int m_lineCount = 0;

    QVector< QPainterPath > m_eraser;
    int m_eraserCount = 0;

    PtrPropertyDeclare(std::function<void(const int &sourceIndex, const QByteArray &imageData)>, onPaintCallback, setOnPaintCallback, = NULL)

public:
    WhiteboardCanvasPainter(WhiteboardCanvasSource *source);

    void setSocket(WhiteboardSocket *socket);

    QByteArray sourceToByteArray(const int &sourceIndex) const;

    void sourceLoadFromByteArray(const int &sourceIndex, const QByteArray &data);

    void sourcePaintFromByteArray(const int &sourceIndex, const QByteArray &data);

public slots:
    void setCurrentSourceIndex(const int &sourceIndex);

    void setPenColor(const QColor &color);

    void setPenWidth(const int &penWidth);

    void setPenOpacity(const double &penOpacity);

    void drawBegin(void);

    void drawPoint(const int &x, const int &y);

    void drawLinePoint(const int &x, const int &y);

    void drawLine(const int &x1, const int &y1, const int &x2, const int &y2);

    void drawLines(void);

    void drawLineEnd(void);

    void drawRectangle(const int &x1, const int &y1, const int &x2, const int &y2);

    void drawEllipse(const int &x1, const int &y1, const int &x2, const int &y2);

    void drawArrow(const int &x1, const int &y1, const int &x2, const int &y2);

    void drawStar(const int &x1, const int &y1, const int &x2, const int &y2);

    void drawHeart(const int &x1, const int &y1, const int &x2, const int &y2);

    void drawText(const int &x, const int &y, const QString &text);

    void drawImage(const int &x1, const int &y1, const int &x2, const int &y2);

    void drawEraserPoint(const int &x, const int &y);

    void drawEraserLine(const int &x1, const int &y1, const int &x2, const int &y2);

    void drawEraserLines(void);

    void drawEraserEnd(void);

    void drawClear(void);

    void drawEnd(void);

    void clearAll(void);

    void clearCover(void);

    void backToHistory(const int &sourceIndex = -1);

    void backToFuture(const int &sourceIndex = -1);

    void save(void);

    void open(void);

    void readyDrawImage(void);

    void onImageSelected(const QImage &image);

signals:
    void imageChanged(void);

    void coverChanged(void);

    void imageSaved(const bool succeed);

    void imageOpened(const bool succeed);
};

#endif//__WhiteboardCanvas_h__
