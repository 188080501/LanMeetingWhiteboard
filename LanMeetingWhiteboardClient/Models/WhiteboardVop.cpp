#include "WhiteboardVop.h"

WhiteboardVop::WhiteboardVop(WhiteboardCanvasPainter *whiteboardCanvasPainter):
    m_whiteboardCanvasPainter(whiteboardCanvasPainter),
    m_vop("54GzQbyspseUfRUbvDdVMKQW", "9dfXhqPz0F6RrpoNLAzMzkBLz4fpHPu9")
{
    m_dictionart.loadFromFile("://Vop/Dictionary.txt");

    QTimer::singleShot(1000, [&]()
    {
        m_vop.refreshToken();
    });
}

void WhiteboardVop::start(void)
{
    m_vop.start();
}

void WhiteboardVop::finish(void)
{
    const auto &&voiceData = m_vop.finish();
    if(!voiceData.first)
    {
        emit messageEmit("听不清");
        return;
    }

    qDebug() << "Voice message:" << voiceData.second;

    const auto &&textData = m_dictionart.find(voiceData.second);
    if(!textData.first)
    {
        emit messageEmit("不理解");
        return;
    }

    switch(textData.second)
    {
        case 1:
        {
            emit messageEmit("你好");
            break;
        }

        case 500:
        {
            m_whiteboardCanvasPainter->backToHistory();
            emit messageEmit("撤销操作");
            break;
        }
        case 505:
        {
            m_whiteboardCanvasPainter->backToFuture();
            emit messageEmit("恢复操作");
            break;
        }
        case 510:
        {
            m_whiteboardCanvasPainter->save();
            emit messageEmit("保存画板");
            break;
        }
        case 515:
        {
            m_whiteboardCanvasPainter->open();
            emit messageEmit("读取画板");
            break;
        }
        case 520:
        {
            emit currentBoardIndexChanged(0);
            break;
        }
        case 521:
        {
            emit currentBoardIndexChanged(1);
            break;
        }
        case 522:
        {
            emit currentBoardIndexChanged(2);
            break;
        }

        case 600:
        {
            emit penColorChanged(QColor(0, 0, 0));
            emit messageEmit("选择了黑色");
            break;
        }
        case 601:
        {
            emit penColorChanged(QColor(255, 255, 255));
            emit messageEmit("选择了白色");
            break;
        }
        case 602:
        {
            emit penColorChanged(QColor(123, 123, 123));
            emit messageEmit("选择了灰色");
            break;
        }
        case 603:
        {
            emit penColorChanged(QColor(255, 0, 0));
            emit messageEmit("选择了红色");
            break;
        }
        case 604:
        {
            emit penColorChanged(QColor(0, 255, 0));
            emit messageEmit("选择了绿色");
            break;
        }
        case 605:
        {
            emit penColorChanged(QColor(0, 0, 255));
            emit messageEmit("选择了蓝色");
            break;
        }
        case 606:
        {
            emit penColorChanged(QColor(255, 255, 0));
            emit messageEmit("选择了黄色");
            break;
        }
        case 607:
        {
            emit penColorChanged(QColor(255, 0, 255));
            emit messageEmit("选择了紫色");
            break;
        }
        case 608:
        {
            emit penColorChanged(QColor(255, 0, 255));
            emit messageEmit("选择了青色");
            break;
        }
        case 609:
        {
            emit penColorChanged(QColor(255, 192, 203));
            emit messageEmit("选择了粉色");
            break;
        }


        case 800:
        {
            emit penOpacityChanged(100);
            emit messageEmit("不透明");
            break;
        }
        case 801:
        {
            emit penOpacityChanged(0);
            emit messageEmit("全透明");
            break;
        }
        case 802:
        {
            emit penOpacityChanged(50);
            emit messageEmit("半透明");
            break;
        }

        case Agreement::PaintPoint + 1000:
        {
            m_whiteboardCanvasPainter->drawBegin();
            m_whiteboardCanvasPainter->drawPoint(320, 548);
            m_whiteboardCanvasPainter->drawEnd();
            emit messageEmit("画一个点");
            break;
        }
        case Agreement::PaintLine + 1000:
        {
            m_whiteboardCanvasPainter->drawBegin();
            m_whiteboardCanvasPainter->drawLine(320 - 100, 548 - 100, 320 + 100, 548 + 100);
            m_whiteboardCanvasPainter->drawEnd();
            emit messageEmit("画一条线");
            break;
        }
        case Agreement::PaintRectangle + 1000:
        {
            m_whiteboardCanvasPainter->drawBegin();
            m_whiteboardCanvasPainter->drawRectangle(320 - 100, 548 - 100, 320 + 100, 548 + 100);
            m_whiteboardCanvasPainter->drawEnd();
            emit messageEmit("画一个矩形");
            break;
        }
        case Agreement::PaintEllipse + 1000:
        {
            m_whiteboardCanvasPainter->drawBegin();
            m_whiteboardCanvasPainter->drawEllipse(320 - 100, 548 - 100, 320 + 100, 548 + 100);
            m_whiteboardCanvasPainter->drawEnd();
            emit messageEmit("画一个圆形");
            break;
        }
        case Agreement::PaintArrow + 1000:
        {
            m_whiteboardCanvasPainter->drawBegin();
            m_whiteboardCanvasPainter->drawArrow(320 - 150, 548 + 150, 320 + 150, 548 - 150);
            m_whiteboardCanvasPainter->drawEnd();
            emit messageEmit("画一个箭头");
            break;
        }

        case Agreement::PaintStar + 1000:
        {
            m_whiteboardCanvasPainter->drawBegin();
            m_whiteboardCanvasPainter->drawStar(320 - 100, 548 - 100, 320 + 100, 548 + 100);
            m_whiteboardCanvasPainter->drawEnd();
            emit messageEmit("画一个星星");
            break;
        }
        case Agreement::PaintHeart + 1000:
        {
            m_whiteboardCanvasPainter->drawBegin();
            m_whiteboardCanvasPainter->drawHeart(320 - 100, 548 - 100, 320 + 100, 548 + 100);
            m_whiteboardCanvasPainter->drawEnd();
            emit messageEmit("画一个爱心");
            break;
        }

        case Agreement::PaintEraser + 1000:
        {
            m_whiteboardCanvasPainter->drawBegin();
            m_whiteboardCanvasPainter->drawClear();
            m_whiteboardCanvasPainter->drawEnd();
            emit messageEmit("清空画板");
            break;
        }

        default:
        {
            emit messageEmit("不理解");
            break;
        }
    }
}
