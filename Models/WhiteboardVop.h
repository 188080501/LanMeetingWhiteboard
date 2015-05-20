#ifndef __WhiteboardVop_h__
#define __WhiteboardVop_h__

// JasonQt lib import
#include "JasonQt_Vop.h"
#include "JasonQt_Trie.h"

// LanMeetingWhiteboard lib import
#include "WhiteboardCanvas.h"

class WhiteboardVop: public QObject
{
    Q_OBJECT

private:
    WhiteboardCanvasPainter *m_whiteboardCanvasPainter;
    JasonQt_Vop::BaiduVop m_vop;
    JasonQt_Trie::Dictionary m_dictionart;

public:
    WhiteboardVop(WhiteboardCanvasPainter *whiteboardCanvasPainter);

public slots:
    void start(void);

    void finish(void);

signals:
    void messageEmit(const QString &message);

    void penColorChanged(const QColor penColor);

    void penOpacityChanged(const int penOpacity);

    void currentBoardIndexChanged(const int &boardIndex);
};

#endif//__WhiteboardVop_h__
