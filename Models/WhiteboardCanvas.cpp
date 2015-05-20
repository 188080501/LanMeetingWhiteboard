#include "WhiteboardCanvas.h"
#include "WhiteboardSocket.h"

// WhiteboardCanvasSource
WhiteboardCanvasSource::WhiteboardCanvasSource(void):
    QQuickImageProvider(QQuickImageProvider::Image)
{
    for(int now = 0; now < WhiteboardMaxSourceSize; now++)
    {
        m_source[now].current = QImage(QSize(640, 1096), QImage::Format_ARGB32);
        m_source[now].cover = QImage(QSize(640, 1096), QImage::Format_ARGB32);
    }
}

QImage WhiteboardCanvasSource::requestImage(const QString &id, QSize *, const QSize &)
{
    if(id.indexOf("source") == 0)
    {
        return m_source[m_currentSourceIndex].current;
    }
    else
    {
        return m_source[m_currentSourceIndex].cover;
    }
}

// WhiteboardCanvasPainter
WhiteboardCanvasPainter::WhiteboardCanvasPainter(WhiteboardCanvasSource *source):
    m_source(source),
    m_penColor(QColor(0, 0, 0)),
    m_drawImageData(":/UI/Disabled.png")
{
#ifdef Q_OS_IOS
    connect(&m_imagePicker, SIGNAL(imageSelected(QImage)), this, SLOT(onImageSelected(QImage)));
    connect(&m_imagePicker, &JasonQt_iOS::ImagePicker::imageSaved, [=](const bool &succeed)
    {
        emit imageSaved(succeed);
    });
#endif

    for(int now = 0; now < WhiteboardMaxSourceSize; now++)
    {
        m_source->m_currentSourceIndex = now;
        this->clearAll();
    }
    m_source->m_currentSourceIndex = 0;
}

void WhiteboardCanvasPainter::setSocket(WhiteboardSocket *socket)
{
    m_socket = socket;
}

QByteArray WhiteboardCanvasPainter::sourceToByteArray(const int &sourceIndex) const
{
    return JasonQt_ImageToByteArray(m_source->m_source[sourceIndex].current, "JPG");
}

void WhiteboardCanvasPainter::sourceLoadFromByteArray(const int &sourceIndex, const QByteArray &data)
{
    m_source->m_source[sourceIndex].current.loadFromData(data, "JPG");

    emit imageChanged();
}

void WhiteboardCanvasPainter::sourcePaintFromByteArray(const int &sourceIndex, const QByteArray &data)
{
    m_mutex.lock();

    if(m_socket->isServer())
    {
        if(m_source->m_source[sourceIndex].history.isEmpty())
        {
            m_source->m_source[sourceIndex].history.push_back(JasonQt_ImageToByteArray(m_source->m_source[sourceIndex].current, "JPG"));
        }
    }

    QImage buf;
    QPainter painter(&m_source->m_source[sourceIndex].current);

    buf.loadFromData(data, "PNG");

    painter.drawImage(0, 0, buf);

    painter.end();

    emit imageChanged();

    if(m_socket->isServer())
    {
        while(m_source->m_source[sourceIndex].historyIndex < (m_source->m_source[sourceIndex].history.size() - 1))
        {
            m_source->m_source[sourceIndex].history.pop_back();
        }
        m_source->m_source[sourceIndex].history.push_back(JasonQt_ImageToByteArray(m_source->m_source[sourceIndex].current, "JPG"));
        if(m_source->m_source[sourceIndex].history.size() > 50)
        {
            m_source->m_source[sourceIndex].history.pop_front();
        }
        m_source->m_source[sourceIndex].historyIndex = m_source->m_source[sourceIndex].history.size() - 1;
    }

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::setCurrentSourceIndex(const int &sourceIndex)
{
    m_source->m_currentSourceIndex = sourceIndex;

    emit imageChanged();
}

void WhiteboardCanvasPainter::setPenColor(const QColor &color)
{
    m_penColor = color;
}

void WhiteboardCanvasPainter::setPenWidth(const int &penWidth)
{
    m_penWidth = penWidth;
}

void WhiteboardCanvasPainter::setPenOpacity(const double &penOpacity)
{
    m_penOpacity = penOpacity;
}

void WhiteboardCanvasPainter::drawBegin(void)
{
    //...
}

void WhiteboardCanvasPainter::drawPoint(const int &x, const int &y)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    const auto flag = ((m_penWidth != -1) ? (m_penWidth) : (6)) * 0.8;

    QPen pen(m_penColor);
    pen.setWidth(flag);
    painter.setPen(pen);

    painter.setOpacity(m_penOpacity);

    painter.drawEllipse(x - (flag / 2), y - (flag / 2), flag, flag);

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawLinePoint(const int &x, const int &y)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    QPen pen(m_penColor);
    pen.setWidth((m_penWidth != -1) ? (m_penWidth) : (3));
    painter.setPen(pen);

    painter.setOpacity(m_penOpacity);

    painter.drawPoint(x, y);

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawLine(const int &x1, const int &y1, const int &x2, const int &y2)
{
    if(m_lineCount > 1000)
    {
        this->drawLineEnd();
        this->drawEnd();
        this->drawBegin();
    }

    m_mutex.lock();

    auto flag = sqrt(sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0)) * 0.50);
    if(flag > 5) { flag = 5; }
    flag = 5 - flag;
    flag = (m_penWidth != -1) ? (m_penWidth) : (flag + 2);

    if(m_line.isEmpty() || (m_line.last().first != (int)flag))
    {
        m_line.push_back( { flag, QPainterPath(QPointF(x1, y1)) } );
    }

    m_line.last().second.lineTo(x2, y2);

    m_lineCount++;

    this->drawLines();

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawLines(void)
{
    this->clearCover();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    QPen pen(m_penColor);
    pen.setJoinStyle(Qt::RoundJoin);

    painter.setOpacity(m_penOpacity);

    painter.setCompositionMode(QPainter::CompositionMode_DestinationAtop);

    for(const auto &now: m_line)
    {
        pen.setWidth(now.first);
        painter.setPen(pen);

        painter.drawPath(now.second);
    }
}

void WhiteboardCanvasPainter::drawLineEnd(void)
{
    m_line.clear();
    m_lineCount = 0;
}

void WhiteboardCanvasPainter::drawRectangle(const int &x1, const int &y1, const int &x2, const int &y2)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    auto flag = sqrt(sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0)) * 0.15);
    if(flag > 6) { flag = 6; }

    QPen pen(m_penColor);
    pen.setWidth((m_penWidth != -1) ? (m_penWidth) : (flag));
    pen.setJoinStyle(Qt::MiterJoin);
    painter.setPen(pen);

    painter.setOpacity(m_penOpacity);

    painter.drawRect(x1, y1, x2 - x1, y2 - y1);

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawEllipse(const int &x1, const int &y1, const int &x2, const int &y2)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    auto flag = sqrt(sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0)) * 0.15);
    if(flag > 6) { flag = 6; }

    QPen pen(m_penColor);
    pen.setWidth((m_penWidth != -1) ? (m_penWidth) : (flag));
    painter.setPen(pen);

    painter.setOpacity(m_penOpacity);

    painter.drawEllipse(x1, y1, x2 - x1, y2 - y1);

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawArrow(const int &x1, const int &y1, const int &x2, const int &y2)
{
    m_mutex.lock();

    const auto &&flag = atan(double(x2 - x1) / double(y2 - y1)) / (M_PI / 2.0) * 360.0;
    int flag2;
    if((x2 - x1) > 0)
    {
        if((y2 - y1) > 0)
        {
            flag2 = 90 + (1 - (flag / 360.0)) * 90.0;
        }
        else
        {
            flag2 = (flag / -360.0) * 90.0;
        }
    }
    else
    {
        if((y2 - y1) > 0)
        {
            flag2 = 180 + ((flag / -360.0) * 90.0);
        }
        else
        {
            flag2 = 270 + (1 - (flag / 360.0)) * 90.0;
        }
    }
    const auto &&flag3 = sqrt(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
    const auto &&flag4 = flag3 * 0.2;
    const auto &&flag5 = (m_penWidth != -1) ? (m_penWidth) : (((flag4 / 5) < 10) ? (flag4 / 5) : (10));

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    QPen pen(m_penColor);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(flag5);
    painter.setPen(pen);

    painter.setOpacity(m_penOpacity);

    painter.translate(x1, y1);
    painter.rotate(flag2);

    painter.drawLine(0, 0, 0, -1 * flag3 + flag5 * 0.3);
    painter.drawLines(QVector<QLine>( {
                                          QLine(0, -1 * flag3, 0 - flag4, -1 * flag3 + flag4),
                                          QLine(0, -1 * flag3, 0 + flag4, -1 * flag3 + flag4)
                                      } ));

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawStar(const int &x1, const int &y1, const int &x2, const int &y2)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    painter.setOpacity(m_penOpacity);

    painter.drawImage(QRect(x1, y1, abs(x2 - x1), abs(y2 - y1)), QImage(":/UI/Star.png"));

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawHeart(const int &x1, const int &y1, const int &x2, const int &y2)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    painter.setOpacity(m_penOpacity);

    painter.drawImage(QRect(x1, y1, abs(x2 - x1), abs(y2 - y1)), QImage(":/UI/Heart.png"));

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawText(const int &x, const int &y, const QString &text)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    painter.setPen(m_penColor);

    painter.setOpacity(m_penOpacity);

    painter.setFont(QFont(painter.font().family(), 40));

    painter.drawText(x - text.size() * 20, y - 20, text.size() * 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, text);

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawImage(const int &x1, const int &y1, const int &x2, const int &y2)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    painter.setOpacity(m_penOpacity);

    painter.drawImage(QRect(x1, y1, abs(x2 - x1), abs(y2 - y1)), m_drawImageData);

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawEraserPoint(const int &x, const int &y)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    QPen pen(QColor(255, 255, 255));
    pen.setWidth((m_penWidth != -1) ? (m_penWidth) : (40));
    painter.setPen(pen);

    painter.setOpacity(m_penOpacity);

    painter.setCompositionMode(QPainter::CompositionMode_DestinationAtop);

    painter.drawPoint(x, y);

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawEraserLine(const int &x1, const int &y1, const int &x2, const int &y2)
{
    if(m_eraserCount > 1000)
    {
        this->drawEraserLines();
        this->drawEnd();
        this->drawBegin();
    }

    m_mutex.lock();

    if(m_eraser.isEmpty())
    {
        m_eraser.push_back(QPainterPath(QPointF(x1, y1)));
    }

    m_eraser.last().lineTo(x2, y2);

    m_eraserCount++;

    this->drawEraserLines();

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawEraserLines(void)
{
    this->clearCover();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    QPen pen(QColor(255, 255, 255));
    pen.setJoinStyle(Qt::RoundJoin);

    painter.setOpacity(m_penOpacity);

    painter.setCompositionMode(QPainter::CompositionMode_DestinationAtop);

    for(const auto &now: m_eraser)
    {
        pen.setWidth((m_penWidth != -1) ? (m_penWidth) : (40));
        painter.setPen(pen);

        painter.drawPath(now);
    }
}

void WhiteboardCanvasPainter::drawEraserEnd(void)
{
    m_eraser.clear();
    m_eraserCount = 0;
}

void WhiteboardCanvasPainter::drawClear(void)
{
    m_mutex.lock();

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].cover);

    painter.setBrush(QColor(255, 255, 255));
    painter.setPen(QColor(255, 255, 255));

    painter.drawRect(QRect({ { 0, 0 }, m_source->m_source[m_source->m_currentSourceIndex].cover.size() }));

    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::drawEnd(void)
{
    m_mutex.lock();

    if(m_onPaintCallback)
    {
        (*m_onPaintCallback)(m_source->m_currentSourceIndex, JasonQt_ImageToByteArray(m_source->m_source[m_source->m_currentSourceIndex].cover, "PNG"));
    }
    if(m_socket->isServer())
    {
        if(m_source->m_source[m_source->m_currentSourceIndex].history.isEmpty())
        {
            m_source->m_source[m_source->m_currentSourceIndex].history.push_back(JasonQt_ImageToByteArray(m_source->m_source[m_source->m_currentSourceIndex].current, "JPG"));
        }
    }

    QPainter painter(&m_source->m_source[m_source->m_currentSourceIndex].current);

    painter.drawImage(0, 0, m_source->m_source[m_source->m_currentSourceIndex].cover);

    painter.end();

    memset(m_source->m_source[m_source->m_currentSourceIndex].cover.bits(), 0, m_source->m_source[m_source->m_currentSourceIndex].cover.byteCount());

    emit imageChanged();
    emit coverChanged();

    if(m_socket->isServer())
    {
        while(m_source->m_source[m_source->m_currentSourceIndex].historyIndex < (m_source->m_source[m_source->m_currentSourceIndex].history.size() - 1))
        {
            m_source->m_source[m_source->m_currentSourceIndex].history.pop_back();
        }
        m_source->m_source[m_source->m_currentSourceIndex].history.push_back(JasonQt_ImageToByteArray(m_source->m_source[m_source->m_currentSourceIndex].current, "JPG"));
        if(m_source->m_source[m_source->m_currentSourceIndex].history.size() > 50)
        {
            m_source->m_source[m_source->m_currentSourceIndex].history.pop_front();
        }
        m_source->m_source[m_source->m_currentSourceIndex].historyIndex = m_source->m_source[m_source->m_currentSourceIndex].history.size() - 1;
    }

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::clearAll(void)
{
    m_mutex.lock();

    m_source->m_source[m_source->m_currentSourceIndex].history.clear();
    m_source->m_source[m_source->m_currentSourceIndex].historyIndex = 0;

    memset(m_source->m_source[m_source->m_currentSourceIndex].current.bits(), 0xff, m_source->m_source[m_source->m_currentSourceIndex].current.byteCount());
    memset(m_source->m_source[m_source->m_currentSourceIndex].cover.bits(), 0, m_source->m_source[m_source->m_currentSourceIndex].cover.byteCount());

    emit imageChanged();
    emit coverChanged();

    m_mutex.unlock();
}

void WhiteboardCanvasPainter::clearCover(void)
{
    memset(m_source->m_source[m_source->m_currentSourceIndex].cover.bits(), 0, m_source->m_source[m_source->m_currentSourceIndex].cover.byteCount());
}

void WhiteboardCanvasPainter::backToHistory(const int &sourceIndex_)
{
    const int sourceIndex = ((sourceIndex_ != -1) ? (sourceIndex_) : (m_source->m_currentSourceIndex));

    if(m_socket->isServer())
    {
        if(!m_source->m_source[sourceIndex].history.isEmpty() && m_source->m_source[sourceIndex].historyIndex)
        {
            m_source->m_source[sourceIndex].current.loadFromData(m_source->m_source[sourceIndex].history[--m_source->m_source[sourceIndex].historyIndex], "JPG");

            emit imageChanged();

            m_socket->sendImageToClient(sourceIndex, m_source->m_source[sourceIndex].history[m_source->m_source[sourceIndex].historyIndex]);
        }
    }
    else
    {
        m_socket->sendBackToHistory(sourceIndex);
    }
}

void WhiteboardCanvasPainter::backToFuture(const int &sourceIndex_)
{
    const int sourceIndex = ((sourceIndex_ != -1) ? (sourceIndex_) : (m_source->m_currentSourceIndex));

    if(m_socket->isServer())
    {
        if(!m_source->m_source[sourceIndex].history.isEmpty() && (m_source->m_source[sourceIndex].historyIndex < (m_source->m_source[sourceIndex].history.size() - 1)))
        {
            m_source->m_source[sourceIndex].current.loadFromData(m_source->m_source[sourceIndex].history[++m_source->m_source[sourceIndex].historyIndex], "JPG");

            emit imageChanged();

            m_socket->sendImageToClient(sourceIndex, m_source->m_source[sourceIndex].history[m_source->m_source[sourceIndex].historyIndex]);
        }
    }
    else
    {
        m_socket->sendBackToFuture(sourceIndex);
    }
}

void WhiteboardCanvasPainter::save(void)
{
#ifdef Q_OS_IOS
    m_imagePicker.save(m_source->m_source[m_source->m_currentSourceIndex].current);
#else
    const auto &&path = QFileDialog::getSaveFileName(NULL, tr("保存画板"), QDir::homePath(), "*.png");
    if(path.isEmpty()) { return; }

    const auto &&flag = m_source->m_source[m_source->m_currentSourceIndex].current.save(path);
    emit imageSaved(flag);
#endif
}

void WhiteboardCanvasPainter::open(void)
{
    m_imagePickerCurrentAction = 1;

#ifdef Q_OS_IOS
    m_imagePicker.show();
#else
    const auto &&path = QFileDialog::getOpenFileName(NULL, tr("读取画板"), QDir::homePath(), "*.png");
    if(path.isEmpty()) { return; }

    QImage image;
    if(!image.load(path)) { emit imageOpened(false); return; }

    this->onImageSelected(image);
#endif
}

void WhiteboardCanvasPainter::readyDrawImage(void)
{
    m_imagePickerCurrentAction = 2;

#ifdef Q_OS_IOS
    m_imagePicker.show();
#else
    const auto &&path = QFileDialog::getOpenFileName(NULL, tr("读取画板"), QDir::homePath(), "*.png");
    if(path.isEmpty()) { return; }

    QImage image;
    if(!image.load(path)) { emit imageOpened(false); return; }

    this->onImageSelected(image);
#endif
}

void WhiteboardCanvasPainter::onImageSelected(const QImage &image)
{
    switch(m_imagePickerCurrentAction)
    {
        case 1:
        {
            if(image.size() != m_source->m_source[m_source->m_currentSourceIndex].current.size())
            {
                m_source->m_source[m_source->m_currentSourceIndex].current = image.scaled(m_source->m_source[m_source->m_currentSourceIndex].current.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
            else
            {
                m_source->m_source[m_source->m_currentSourceIndex].current = image;
            }

            emit imageChanged();
            emit imageOpened(true);
            break;
        }
        case 2:
        {
            m_drawImageData = image;
            break;
        }
    }

    m_imagePickerCurrentAction = 0;
}
