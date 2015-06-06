/*
    JasonQt_iOS

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

#ifndef __JasonQt_iOS_h__
#define __JasonQt_iOS_h__

// Qt lib import
#include <QObject>
#include <QImage>
#include <QString>
#include <QDateTime>
#include <QBuffer>

namespace JasonQt_iOS
{

void pushNotifiction(const QString &message, const QDateTime &dateTime);

void cancelAllNotifictions(void);

class ImagePicker: public QObject
{
    Q_OBJECT

public:
    static ImagePicker *g_currentImagePicker;
    static void *g_context;

public slots:
    void show(void);

    void save(const QImage &image);

signals:
    void imageSelected(const QImage image);

    void imageSaved(const bool succeed);
};

}

#endif//__JasonQt_iOS_h__
