#include "JasonQt_Foundation.h"

// Funcation define
int Jason_GetSystemTime(void)
{
    static bool flag = true;
    static QTime timer;
    if(flag)
    {
        timer.start();
        flag = false;
    }
    return timer.elapsed();
}

void JasonQt_EventLoopSleep(const int &delay)
{
    QEventLoop eventLoop;
    QTimer::singleShot(delay, &eventLoop, SLOT(quit()));
    eventLoop.exec();
}

void JasonQt_ForeachWidget(QWidget *parent, const std::function<void (QWidget *)> &each, const bool &recursive)
{
    QObjectList children = parent->children();
    for(auto &now: children)
    {
        if(now->inherits("QWidget"))
        {
            each(qobject_cast<QWidget *>(now));
            if(recursive)
            {
                JasonQt_ForeachWidget(parent, each, recursive);
            }
        }
    }
}

void JasonQt_TableWidget_SetHorizontalHeaderLabels(QTableWidget *tableWidget, const QStringList &stringList)
{
    tableWidget->setColumnCount(stringList.size());
    tableWidget->setHorizontalHeaderLabels(stringList);
}

void JasonQt_TableWidget_SetColumnWidth(QTableWidget *tableWidget, const QVector<int> &columnWidth)
{
    int Column = -1;
    foreach(auto &&Now, columnWidth)
    {
        tableWidget->setColumnWidth(++Column, Now);
    }
}

void JasonQt_TableWidget_AddString(QTableWidget *tablwWidget, const QStringList &stringList)
{
    int x = 0, y = tablwWidget->rowCount();
    tablwWidget->setRowCount(y + 1);
    QTableWidgetItem *Item;

    foreach(QString Now, stringList)
    {
        Item = new QTableWidgetItem(Now);
        Item->setSelected(false);
        Item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tablwWidget->setItem(y, x++, Item);
    }
}

void JasonQt_TreeWidget_SetHorizontalHeaderLabels(QTreeWidget *treeWidget, const QStringList &stringList)
{
    treeWidget->setColumnCount(stringList.size());
    treeWidget->setHeaderLabels(stringList);
}

void JasonQt_TreeWidget_SetColumnWidth(QTreeWidget *treeWidget, const QVector<int> &columnWidth)
{
    int Column = -1;
    foreach(auto &&Now, columnWidth)
    {
        ++Column;
        if(Now > 0)
        {
            treeWidget->setColumnWidth(Column, Now);
        }
        else if(!Now)
        {
            continue;
        }
        else
        {
            treeWidget->setColumnWidth(Column, treeWidget->headerItem()->text(Column).size() * abs(Now));
        }
    }
}

void JasonQt_SetWidgetColor(QWidget *label, const QColor &color)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    label->setPalette(palette);
}

void JasonQt_TexetEditMoveCursorToEnd(QTextEdit *textEdit)
{
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
}

void JasonQt_TextEditInsertTextToEnd(QTextEdit *textEdit, const QString &string)
{
    textEdit->insertPlainText(string);
    JasonQt_TexetEditMoveCursorToEnd(textEdit);
}

QString JasonQt_HashString(const QString &Key)
{
    return JasonQt_HashString(Key.toLatin1());
}

QString JasonQt_HashString(const QByteArray &Key)
{
    return QCryptographicHash::hash(Key, QCryptographicHash::Sha1).toHex();
}

QPoint JasonQt_GetWidget_GlobalPos(const QWidget *widget)
{
    if(!widget)
    {
        return QPoint(-1,-1);
    }
    float px = 0;
    float py = 0;
    QWidget const *Parent = widget;
    QWidget const *PreParent = NULL;

    do
    {
        QPoint p = Parent->pos();
        px += p.x();
        py += p.y();
        PreParent = Parent;
        Parent = Parent->parentWidget();
    }
    while(Parent);

    QSize &&Size = PreParent->frameSize();
    QSize &&Size2 = PreParent->size();
    px = px + (Size.width() - Size2.width()) / 2;
    py = py + (Size.height() - Size2.height() - (Size.width() - Size2.width()) / 2);
    QPoint pr(px, py);
    return pr;
}

QRect JasonQt_GetWidget_GlobalGeometry(const QWidget *widget)
{
    return QRect(JasonQt_GetWidget_GlobalPos(widget), widget->size());
}

QWidget *JasonQt_TopParentWidget(QWidget *widget)
{
    QWidget *Parent = widget;

    while(Parent->parentWidget())
    {
        Parent = Parent->parentWidget();
    }

    return Parent;
}

const QWidget *JasonQt_TopParentWidget(const QWidget *widget)
{
    QWidget const *Parent = widget;

    while(Parent->parentWidget())
    {
        Parent = Parent->parentWidget();
    }

    return Parent;
}

#if !(defined Q_OS_IOS) && !(defined Q_OS_ANDROID) && !(defined Q_OS_WINPHONE)
bool JasonQt_SingleApplication(const QString &flag)
{
    static QSharedMemory *shareMem = NULL;

    if(shareMem)
    {
        return true;
    }

    shareMem = new QSharedMemory("JasonQt_SingleApplication_Control_" + flag);

    volatile short i = 2;
    while (i--)
    {
        if (shareMem->attach(QSharedMemory::ReadOnly))
        {
            shareMem->detach();
        }
    }

    if(shareMem->create(1))
    {
        return true;
    }

    return false;
}
#else
bool JasonQt_SingleApplication(const QString &)
{
    return true;
}
#endif

QString JasonQt_ByteArray_To_HexString(const QByteArray &data)
{
    QString buf(data.toHex());
    for(int Now = 1; Now < data.size(); Now++)
    {
        buf.insert(Now * 2 + Now - 1, ' ');
    }
    return buf;
}

QByteArray JasonQt_PixmapToByteArray(const QPixmap &pixmap, const QString &format)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, format.toLatin1().data());
    return bytes;
}

QByteArray JasonQt_ImageToByteArray(const QImage &image, const QString &format)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, format.toLatin1().data());
    return bytes;
}

QPixmap JasonQt_ByteArrayToPixmap(const QByteArray &byteArray)
{
    QPixmap Pixmap;
    Pixmap.loadFromData(byteArray);
    return Pixmap;
}

void JasonQt_LineEdit_SetTo_IPLineEdit(QLineEdit *lineEdit)
{
    lineEdit->setValidator(new QRegExpValidator(QRegExp("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])[.](\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])[.](\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])[.](\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$")));
}

void JasonQt_LineEdit_SetTo_azAZ09LineEdit(QLineEdit *lineEdit)
{
    lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9a-zA-Z]+")));
}

void JasonQt_LineEdit_SetTo_09LineEdit(QLineEdit *lineEdit)
{
    lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
}

void JasonQt_Widget_SetToTransparent(QWidget *target)
{
    target->setAttribute(Qt::WA_TranslucentBackground, true);
    target->setAttribute(Qt::WA_NoSystemBackground, false);
    target->setWindowFlags(Qt::FramelessWindowHint);
    target->setStyleSheet(target->styleSheet() + QString("\nQWidget#%1 { background-color: transparent; }").arg(target->objectName()));
    target->setUpdatesEnabled(true);
}

// Class define
JasonQt_Background_Class::JasonQt_Background_Class(void)
{
    m_backgroundModeMode = BackgroundMode_OnlyOne;
}

void JasonQt_Background_Class::paintEvent(QPaintEvent *)
{
    //对现存背景图片的合理性进行检查
    if(!(m_background.width() && m_background.height()))
    {
        return;
    }

    //开始绘制背景
    QPainter Paint(this);
    switch(m_backgroundModeMode)
    {
        case BackgroundMode_OnlyOne:
        {
            Paint.drawImage(0, 0, m_background);
            break;
        }
        case BackgroundMode_Filling:
        {
            for(int y = 0; y < this->height(); y += m_background.height())
            {
                for(int x = 0; x < this->width(); x += m_background.width())
                {
                    Paint.drawImage(x, y, m_background);
                }
            }
            break;
        }
        case BackgroundMode_Tensile:
        {
            Paint.drawImage(0, 0, m_background);
            break;
        }
    }
}

void JasonQt_Background_Class::Set_Background(QWidget *Parent, const QString &Background, const int BackgroudMode, const int x, const int y)
{
    this->setParent(Parent);
    this->resize((x) ? (x) : (Parent->width()), (y) ? (y) : (Parent->height()));
    this->lower();
    m_backgroundModeMode = BackgroudMode;
    switch(m_backgroundModeMode)
    {
        case BackgroundMode_Tensile:
        {
            m_background.load(Background);
            m_background = m_background.scaled(x, y);
            break;
        }
        default:
        {
            m_background.load(Background);
            break;
        }
    }
}

void JasonQt_Background_Class::Set_Background(QWidget *Parent, const QImage &Background, const int BackgroudMode, const int x, const int y)
{
    this->setParent(Parent);
    this->resize((x) ? (x) : (Parent->width()), (y) ? (y) : (Parent->height()));
    this->lower();
    m_backgroundModeMode = BackgroudMode;
    switch(m_backgroundModeMode)
    {
        case BackgroundMode_Tensile:
        {
            m_background = Background.scaled(x, y);
            break;
        }
        default:
        {
            m_background = Background;
            break;
        }
    }
}

// JasonQt_BeforMain
JasonQt_BeforMain::JasonQt_BeforMain(const std::function<void ()> &Funcation)
{
    Funcation();
    delete this;
}

// JasonQt_ReadyExit
JasonQt_ReadyExit::JasonQt_ReadyExit(const QString &message):
    m_message(message)
{ }

// JasonQt_KeyEvent
JasonQt_KeyEvent_Class::JasonQt_KeyEvent_Class(void):
    m_MinInterval(25),
    m_MaxInterval(1000)
{
    connect(&m_TimeControl, SIGNAL(timeout()), this, SLOT(Inspection()));
}

void JasonQt_KeyEvent_Class::Set_MinInterval(const int &Interval)
{
    m_MinInterval = ((Interval > 1) ? (Interval) : (1));
}

void JasonQt_KeyEvent_Class::Set_MaxInterval(const int &Interval)
{
    if(Interval < m_MinInterval)
    {
        qDebug("Set_MaxInterval Error! -> %d", Interval);
        return;
    }
    else
    {
        m_MaxInterval = ((Interval > 1000) ? (Interval) : (1000));
    }
}

void JasonQt_KeyEvent_Class::Inspction_Press(const int Key)
{
    if(m_PressKey.find(Key) == m_PressKey.end()) //查看映射中有没有记录
    {
        buf_ul = Jason_GetSystemTime();
        m_PressKey[Key] = ((buf_ul) ? (buf_ul) : (1));
        emit SIGNAL_KeyPress_One(Key); //如果没有记录，则加入映射并且发出按下信号（此信号除非释放按键否则只会出一次）
        emit SIGNAL_KeyPress_Multi(Key);
    }
    else
    {
        m_PressKey[Key] = Jason_GetSystemTime() - m_MinInterval - 1; //如果有，则标记为可以立刻返回Multi
    }
    if(!m_TimeControl.isActive())
    {
        m_TimeControl.start(m_MinInterval); //如果没有启动定时器则启动定时器
    }
}

void JasonQt_KeyEvent_Class::Inspction_Release(const int Key)
{
    if(m_PressKey.find(Key) != m_PressKey.end())
    {
        m_PressKey[Key] = 0; //如果映射中有当前需要释放的按键的记录，则标记为0
    }
}

void JasonQt_KeyEvent_Class::Inspection(void)
{
    buf_ul = Jason_GetSystemTime();
    for(std::map<int, unsigned long>::iterator Now = m_PressKey.begin(); Now != m_PressKey.end(); Now ++) //遍历映射
    {
        if(Now->second)
        {
            if((int)(buf_ul - Now->second) > m_MaxInterval) //超时，标记为需要释放
            {
                m_WaitRelease.push_back(Now->first);
            }
            else if((int)(buf_ul - Now->second) > m_MinInterval) //到达了最短信号发送时间则发送信号并且刷新
            {
                emit SIGNAL_KeyPress_Multi(Now->first);
            }
        }
        else //如果标记为0表示需要释放
        {
            m_WaitRelease.push_back(Now->first);
            continue;
        }
    }
    if(m_WaitRelease.size()) //释放所有需要释放的按键
    {
        m_PressKey.erase(m_PressKey.find(*m_WaitRelease.begin()));
        emit SIGNAL_KeyRelease(*m_WaitRelease.begin());
        m_WaitRelease.erase(m_WaitRelease.begin());
    }
}

// JasonQt_MouseEvent_Class
bool JasonQt_MouseEvent_Class::Is_Press(void) const
{
    return m_Press;
}

void JasonQt_MouseEvent_Class::Inspection_Press(const QPoint &Pos)
{
    m_Source = Pos;
    m_Now = Pos;
    m_Press = true;
    emit SIGNAL_MousePress(Pos);
}

void JasonQt_MouseEvent_Class::Inspection_Move(const QPoint &Pos)
{
    const auto &&Now = Pos - m_Now;
    const auto &&Sum = Pos - m_Source;
    m_Now = Pos;

    emit SIGNAL_MouseMove(Pos);
    emit SIGNAL_MouseMove_NowDisplacement(m_Source, Pos, Now);
    emit SIGNAL_MouseMove_SumDisplacement(m_Source, Pos, Sum);
}

void JasonQt_MouseEvent_Class::Inspection_Release(const QPoint &Pos)
{
    const auto &&Sum = Pos - m_Source;
    m_Press = false;

    emit SIGNAL_MouseRelease(Pos);
    emit SIGNAL_MouseRelease_SumDisplacement(m_Source, Pos, Sum);
}

// JasonQt_MouseEventEx_Class
JasonQt_MouseEventEx_Class::JasonQt_MouseEventEx_Class(void)
{
    connect(this, SIGNAL(SIGNAL_MousePress(QPoint)), this, SLOT(MousePress(QPoint)));
    connect(this, SIGNAL(SIGNAL_MouseMove_NowDisplacement(QPoint,QPoint,QPoint)), this, SLOT(MouseMove_NowDisplacement(QPoint,QPoint,QPoint)));
    connect(this, SIGNAL(SIGNAL_MouseRelease(QPoint)), this, SLOT(MouseRelease(QPoint)));
}

void JasonQt_MouseEventEx_Class::Add_MoveTrusteeship(QWidget *Target)
{
    m_MoveTrusteeship.push_back(Target);
}

bool JasonQt_MouseEventEx_Class::Delete_MoveTrusteeship(QWidget *Target)
{
    for(auto Now = m_MoveTrusteeship.begin(); Now != m_MoveTrusteeship.end(); Now++)
    {
        if(*Now == Target)
        {
            m_MoveTrusteeship.erase(Now);

            if(*Now == m_Now)
            {
                m_Now = NULL;
            }

            return true;
        }
    }
    return false;
}

void JasonQt_MouseEventEx_Class::Clear_MoveTrusteeship(void)
{
    m_MoveTrusteeship.clear();
}

void JasonQt_MouseEventEx_Class::MousePress(const QPoint Now)
{
    foreach(QWidget *Widget, m_MoveTrusteeship)
    {
        if(Widget->geometry().contains(Now))
        {
            m_Now = Widget;
            return;
        }
    }
}

void JasonQt_MouseEventEx_Class::MouseMove_NowDisplacement(const QPoint, const QPoint, const QPoint Displacement)
{
    if(m_Now)
    {
        const auto &&Previous = m_Now->pos();
        const auto &&Current = Previous + Displacement;

        m_Now->move(m_Now->pos() + Displacement);
        emit SIGNAL_TargetMove(m_Now, Current, Previous);
    }
}

void JasonQt_MouseEventEx_Class::MouseRelease(const QPoint)
{
    if(m_Now)
    {
        m_Now = NULL;
    }
}

// JasonQt_ShowInformationMessageBoxFromOtherThread
JasonQt_ShowInformationMessageBoxFromOtherThread::JasonQt_ShowInformationMessageBoxFromOtherThread(const QString &title, const QString &message):
    m_title(title),
    m_message(message)
{ }

void JasonQt_ShowInformationMessageBoxFromOtherThread::show(const QString &title, const QString &message)
{
    QEventLoop eventLoop;
    auto messageBox = new JasonQt_ShowInformationMessageBoxFromOtherThread(title, message);
    connect(messageBox, SIGNAL(destroyed()), &eventLoop, SLOT(quit()));
    messageBox->readyShow();
    eventLoop.exec();
}

void JasonQt_ShowInformationMessageBoxFromOtherThread::readyShow(void)
{
    this->moveToThread(qApp->thread());
    QTimer::singleShot(0, this, SLOT(onShow()));
}

void JasonQt_ShowInformationMessageBoxFromOtherThread::onShow(void)
{
    QMessageBox::information(NULL, m_title, m_message);
    this->deleteLater();
}

QByteArray JasonQt_SingleJson(const QString &key, const QJsonValue &value)
{
    QJsonObject data;
    data.insert(key, value);
    return QJsonDocument(data).toJson();
}

QByteArray JasonQt_MultiJson(QList<std::pair<QString, QJsonValue> > &json)
{
    QJsonObject data;
    for(const auto &now: json)
    {
        data.insert(now.first, now.second);
    }
    return QJsonDocument(data).toJson();
}
