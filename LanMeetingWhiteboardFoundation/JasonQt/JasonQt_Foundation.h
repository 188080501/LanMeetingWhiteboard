/*
    JasonQt_Foundation

    Copyright: Jason

    Contact email: 188080501@qq.com

    GNU Lesser General Public License Usage
    Alternatively, this file may be used under the terms of the GNU Lesser
    General Public License version 2.1 or version 3 as published by the Free
    Software Foundation and appearing in the file LICENSE.LGPLv21 and
    LICENSE.LGPLv3 included in the packaging of this file. Please review the
    following information to ensure the GNU Lesser General Public License
    requirements will be met: https://www.gnu.org/licenses/lgpl.html and
    http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*/

#ifndef __JasonQt_Foundation_h__
#define __JasonQt_Foundation_h__

// C++ lib import
#include <map>
#include <vector>
#include <functional>
#include <array>

// Qt lib import
#include <QApplication>
#include <QObject>
#include <QCryptographicHash>
#include <QMutex>
#include <QSettings>
#include <QSharedMemory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QBuffer>
#include <QThread>
#include <QDateTime>
#include <QTime>
#include <QHash>
#include <QVariant>

#include <QKeyEvent>
#include <QImage>
#include <QPainter>
#include <QDesktopWidget>
#include <QAbstractButton>
#include <QTextCursor>
#include <QTabWidget>
#include <QTabBar>
#include <QTableWidget>
#include <QTreeWidget>
#include <QWidget>
#include <QMessageBox>
#include <QPalette>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>

// Macro define
#define SingleNew(p, ...)                                           \
    if(p) { delete p;  }                                            \
    p = new decltype(*p)(__VA_ARGS__);

#define PropertyDeclare(Type, Name, setName, ...)                   \
    private:                                                        \
    Type m_ ## Name __VA_ARGS__;                                    \
    public:                                                         \
    inline const Type &Name(void) const { return m_ ## Name; }      \
    inline void setName(const Type &Name) { m_ ## Name = Name; }    \
    private:

#define PtrPropertyDeclare(Type, Name, setName, ...)                \
    private:                                                        \
    Type *m_ ## Name __VA_ARGS__;                                   \
    public:                                                         \
    inline const Type *Name(void) const                             \
        { return m_ ## Name; }                                      \
    inline void setName(const Type &Name)                           \
        { if(m_ ## Name) { delete m_ ## Name; }                     \
        m_ ## Name = new Type(Name); }                              \
    private:

#define PrimaryKeyPropertyDeclare(ClassName, Type, Name, ...)       \
    Type Name __VA_ARGS__;                                          \
    ClassName(void) = default;                                      \
    ClassName(const Type &Name ## _): Name(Name ## _) { }           \
    operator QVariant(void) const { return Name; }                  \
    operator Type(void) const { return Name; }

#define rforeach(variable, container)                                                   \
for(JasonQt_RforeachContainer<__typeof__((container))> _container_((container));        \
    _container_.__howMuch__; _container_.__howMuch__--)                                 \
    for(variable = *(--_container_.__now__); ; __extension__ ({ break;}))

template <typename T>
class JasonQt_RforeachContainer {
public:
    inline JasonQt_RforeachContainer(const T& t) : __now__(t.end()), __howMuch__(t.size()){ }
    typename T::const_iterator __now__;
    int __howMuch__;
};

// Funcation define
int Jason_GetSystemTime(void);

void JasonQt_EventLoopSleep(const int &delay);

void JasonQt_ForeachWidget(QWidget *parent, const std::function<void(QWidget *)> &each, const bool &recursive = false);

void JasonQt_TableWidget_SetHorizontalHeaderLabels(QTableWidget *tableWidget, const QStringList &stringList);

void JasonQt_TableWidget_SetColumnWidth(QTableWidget *tableWidget, const QVector<int> &columnWidth);

void JasonQt_TableWidget_AddString(QTableWidget *tablwWidget, const QStringList &string);

void JasonQt_TreeWidget_SetHorizontalHeaderLabels(QTreeWidget *treeWidget, const QStringList &ttringList);

void JasonQt_TreeWidget_SetColumnWidth(QTreeWidget *treeWidget, const QVector<int> &columnWidth);

void JasonQt_SetWidgetColor(QWidget *label, const QColor &color);

void JasonQt_TexetEditMoveCursorToEnd(QTextEdit *textEdit);

void JasonQt_TextEditInsertTextToEnd(QTextEdit *textEdit, const QString &string);

QString JasonQt_HashString(const QString &Key);

QString JasonQt_HashString(const QByteArray &Key);

QPoint JasonQt_GetWidget_GlobalPos(const QWidget *widget);

QRect JasonQt_GetWidget_GlobalGeometry(const QWidget *widget);

QWidget *JasonQt_TopParentWidget(QWidget *widget);

const QWidget *JasonQt_TopParentWidget(const QWidget *widget);

bool JasonQt_SingleApplication(const QString &flag);

QString JasonQt_ByteArray_To_HexString(const QByteArray &data);

QByteArray JasonQt_PixmapToByteArray(const QPixmap &pixmap, const QString &format);

QByteArray JasonQt_ImageToByteArray(const QImage &image, const QString &format);

QPixmap JasonQt_ByteArrayToPixmap(const QByteArray &byteArray);

void JasonQt_LineEdit_SetTo_IPLineEdit(QLineEdit *lineEdit);

void JasonQt_LineEdit_SetTo_azAZ09LineEdit(QLineEdit *lineEdit);

void JasonQt_LineEdit_SetTo_09LineEdit(QLineEdit *lineEdit);

void JasonQt_Widget_SetToTransparent(QWidget *target);

QByteArray JasonQt_SingleJson(const QString &key, const QJsonValue &value);

QByteArray JasonQt_MultiJson(QList<std::pair<QString, QJsonValue> > &json);

// Template funcation define
template<typename A>
void JasonQt_ReleaseMemory(QVector<A *> *Data);

// Class define
class JasonQt_Background_Class: public QWidget
{
    Q_OBJECT

private:
    QImage m_background;
    int m_backgroundModeMode;

public:
    enum BackgroundMode
    {
        BackgroundMode_OnlyOne,
        BackgroundMode_Filling,
        BackgroundMode_Tensile
    };

public:
    JasonQt_Background_Class(void);

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void Set_Background(QWidget *Parent, const QString &Background, const int BackgroudMode = BackgroundMode_Tensile, const int x = 0, const int y = 0);

    void Set_Background(QWidget *Parent, const QImage &Background, const int BackgroudMode = BackgroundMode_Tensile, const int x = 0, const int y = 0);
};

class JasonQt_BeforMain
{
public:
    JasonQt_BeforMain(const std::function<void(void)> &Funcation);
};

class JasonQt_ReadyExit
{
private:
    QString m_message;

public:
    JasonQt_ReadyExit(const QString &message);

    inline const QString &message(void) const { return m_message; }

    operator QString(void) const { return m_message; }
};

class JasonQt_KeyEvent_Class: public QObject
{
    Q_OBJECT

private:
    std::map<int, unsigned long> m_PressKey;
    std::vector<int> m_WaitRelease;
    QTimer m_TimeControl;
    int m_MinInterval;
    int m_MaxInterval;
    unsigned long buf_ul;

public:
    JasonQt_KeyEvent_Class(void);

public slots:
    void Set_MinInterval(const int &Interval);

    void Set_MaxInterval(const int &Interval);

    void Inspction_Press(const int Key);

    void Inspction_Release(const int Key);

private slots:
    void Inspection(void);

signals:
    void SIGNAL_KeyPress_One(const int Key);

    void SIGNAL_KeyPress_Multi(const int Key);

    void SIGNAL_KeyRelease(const int Key);
};

#define JasonQt_KeyEvent_Define(KeyEvent_Name)                          \
private:                                                                \
                                                                        \
JasonQt_KeyEvent_Class KeyEvent_Name;                                   \
                                                                        \
void keyPressEvent(QKeyEvent *e)                                        \
{                                                                       \
    KeyEvent_Name.Inspction_Press(e->key());                            \
}                                                                       \
                                                                        \
void keyReleaseEvent(QKeyEvent *e)                                      \
{                                                                       \
    KeyEvent_Name.Inspction_Release(e->key());                          \
}

class JasonQt_MouseEvent_Class: public QObject
{
    Q_OBJECT

private:
    QPoint m_Source;
    QPoint m_Now;
    bool m_Press = false;

public:
    JasonQt_MouseEvent_Class(void) = default;

    ~JasonQt_MouseEvent_Class(void) = default;

public:
    bool Is_Press(void) const;

public slots:
    void Inspection_Press(const QPoint &Pos);

    void Inspection_Move(const QPoint &Pos);

    void Inspection_Release(const QPoint &Pos);

signals:
    void SIGNAL_MousePress(const QPoint Pos);

    void SIGNAL_MouseMove(const QPoint Pos);

    void SIGNAL_MouseMove_NowDisplacement(const QPoint Source, const QPoint Now, const QPoint Displacement);

    void SIGNAL_MouseMove_SumDisplacement(const QPoint Source, const QPoint Now, const QPoint Displacement);

    void SIGNAL_MouseRelease(const QPoint Pos);

    void SIGNAL_MouseRelease_SumDisplacement(const QPoint Source, QPoint Now, const QPoint Displacement);
};

#define JasonQt_MouseEvent_Define(MouseEvent_Name)                      \
private:                                                                \
                                                                        \
JasonQt_MouseEvent_Class MouseEvent_Name;                               \
                                                                        \
void mousePressEvent(QMouseEvent *e)                                    \
{                                                                       \
    MouseEvent_Name.Inspection_Press(e->pos());                         \
}                                                                       \
                                                                        \
void mouseMoveEvent(QMouseEvent *e)                                     \
{                                                                       \
    MouseEvent_Name.Inspection_Move(e->pos());                          \
}                                                                       \
                                                                        \
void mouseReleaseEvent(QMouseEvent *e)                                  \
{                                                                       \
    MouseEvent_Name.Inspection_Release(e->pos());                       \
}

#define JasonQt_MouseEvent_GlobalDefine(MouseEvent_Name)                \
private:                                                                \
                                                                        \
JasonQt_MouseEvent_Class MouseEvent_Name;                               \
                                                                        \
void mousePressEvent(QMouseEvent *e)                                    \
{                                                                       \
    MouseEvent_Name.Inspection_Press(e->globalPos());                   \
}                                                                       \
                                                                        \
void mouseMoveEvent(QMouseEvent *e)                                     \
{                                                                       \
    MouseEvent_Name.Inspection_Move(e->globalPos());                    \
}                                                                       \
                                                                        \
void mouseReleaseEvent(QMouseEvent *e)                                  \
{                                                                       \
    MouseEvent_Name.Inspection_Release(e->globalPos());                 \
}

class JasonQt_MouseEventEx_Class: public JasonQt_MouseEvent_Class
{
    Q_OBJECT

private:
    QVector<QWidget *> m_MoveTrusteeship;
    QWidget *m_Now = NULL;

public:
    JasonQt_MouseEventEx_Class(void);

    ~JasonQt_MouseEventEx_Class(void) = default;

public slots:
    void Add_MoveTrusteeship(QWidget *Target);

    bool Delete_MoveTrusteeship(QWidget *Target);

    void Clear_MoveTrusteeship(void);

private slots:
    void MousePress(const QPoint Now);

    void MouseMove_NowDisplacement(const QPoint, const QPoint Now, const QPoint Displacement);

    void MouseRelease(const QPoint Now);

signals:
    void SIGNAL_TargetMove(QWidget *Target, const QPoint Current, const QPoint Previous);
};

#define JasonQt_MouseEventEx_Define(MouseEventEx_Name)                  \
private:                                                                \
                                                                        \
JasonQt_MouseEventEx_Class MouseEventEx_Name;                           \
                                                                        \
void mousePressEvent(QMouseEvent *e)                                    \
{                                                                       \
    MouseEventEx_Name.Inspection_Press(e->pos());                       \
}                                                                       \
                                                                        \
void mouseMoveEvent(QMouseEvent *e)                                     \
{                                                                       \
    MouseEventEx_Name.Inspection_Move(e->pos());                        \
}                                                                       \
                                                                        \
void mouseReleaseEvent(QMouseEvent *e)                                  \
{                                                                       \
    MouseEventEx_Name.Inspection_Release(e->pos());                     \
}

// JasonQt_ShowInformationMessageBoxFromOtherThread
class JasonQt_ShowInformationMessageBoxFromOtherThread: public QObject
{
    Q_OBJECT

private:
    const QString m_title;
    const QString m_message;

public:
    JasonQt_ShowInformationMessageBoxFromOtherThread(const QString &title, const QString &message);

    static void show(const QString &title, const QString &message);

private:
    void readyShow(void);

private slots:
    void onShow(void);
};

template<class A>
class JasonQt_BlockWidget_Class
{
private:
    A m_ExitData;
    QEventLoop m_EventLoop;

public:
    JasonQt_BlockWidget_Class(void) = default;

    JasonQt_BlockWidget_Class(A Default_ExitData);

    void Exit(A Exitdata);

    A Exec(void);

    void Set_ExitData(A ExitData);

    A ExitData(void);
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TemplateFunctionDefine~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// JasonQt_BlockWidget_Class
template<class A>
JasonQt_BlockWidget_Class<A>::JasonQt_BlockWidget_Class(A Default_ExitData)
{
    m_ExitData = Default_ExitData;
}

template<class A>
void JasonQt_BlockWidget_Class<A>::Exit(A Exitdata)
{
    m_ExitData = Exitdata;
    m_EventLoop.exit();
}

template<class A>
A JasonQt_BlockWidget_Class<A>::Exec(void)
{
    m_EventLoop.exec();
    return m_ExitData;
}

template<class A>
void JasonQt_BlockWidget_Class<A>::Set_ExitData(A ExitData)
{
    m_ExitData = ExitData;
}

template<class A>
A JasonQt_BlockWidget_Class<A>::ExitData(void)
{
    return m_ExitData;
}

#endif//__JasonQt_Foundation_h__
