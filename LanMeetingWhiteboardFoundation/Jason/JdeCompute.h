/*
    JDE (Jason Data Encryption)

    Create: 2015-04-14

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

#ifndef __JdeCompute_h__
#define __JdeCompute_h__

/*
    On my computer( MacBook Pro (Retina, 13-inch, Mid 2014) and Qt 5.4.1 ), run approximately as follows:
    encryption: 163MB/s
    decrypt: 163MB/s
*/

// Jde lib import
#include "JdeKey.h"

namespace Jde
{

void encryption(const void *source, const int &size, void *target, Jde::Key *key = NULL);

void decrypt(const void *source, const int &size, void *target, Jde::Key *key = NULL);

}

#endif//__JdeCompute_h__
