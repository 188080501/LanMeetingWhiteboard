/*
    JDN (Jason Data Notation)

    Create: 2015-04-03

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

#ifndef __JdnDefine_h__
#define __JdnDefine_h__

#ifndef QT_CORE_LIB
#   define JdnSetNodeMemoryMaxSize          8
#   define JdnSetDataMemoryMinSize          8
#   define JdnSetDataMemoryMaxSize          64
#   define JdnNodeDataMaxSize               64
#   define JdnNodeCompressionStartingSize   32
#   define JdnNodeCompressionStartingRatio  0.90f
#   define JdnStreamMaxSize                 1024
#else
#   define JdnSetNodeMemoryMaxSize          2048
#   define JdnSetDataMemoryMinSize          128
#   define JdnSetDataMemoryMaxSize          128 * 1024
#   define JdnNodeDataMaxSize               (8 * 1024 * 1024)
#   define JdnNodeCompressionStartingSize   128
#   define JdnNodeCompressionStartingRatio  0.95f
#   define JdnStreamMaxSize                 16 * 1024 * 1024
#endif

// Jde lib import
#include "Jde.h"

namespace Jdn
{

enum NodeType
{
    UnKnowNode                              = 0x0,
    OnlyNumNode                             = 0x1,
    OnlyStringNode                          = 0x2,
    OnlyByteArrayNode                       = 0x3,
    WithIntNode                             = 0x4,
    WithStringNode                          = 0x5,
    WithByteArrayNode                       = 0x6,
    NodeTypeRange                           = 0x7,
};

enum NodeDataFlag
{
    UseValidationFlag                       = 0x10,
    NodeMemoryOwnerFlag                     = 0x20,
    DataMemoryOwnerFlag                     = 0x40
};

enum NodeStreamFlag
{
    NodeStreamStartFlag                     = 0x80,
    NodeStreamNumCompressionFlag            = 0x40,
    NodeStreamIntDataCompressionFlag        = 0x20,
    NodeStreamUseValidationFlag             = 0x10
};

enum StreamSizeFlag
{
    StreamSizeJustFine                      = 0,
    StreamSizeNeedWaitMore                  = 1,
    StreamSizeOverTargetSize                = 2,
    StreamSizeOverMaxSize                   = 3
};

}

#endif//__JdnDefine_h__
