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

#ifndef __JdeKey_h__
#define __JdeKey_h__

// Jde lib import
#include "JdeDefine.h"

namespace Jde
{

#pragma pack(push)
#pragma pack(1)

class Key
{
private:
    union
    {
        uint8_t allData[42];
        struct
        {
            uint8_t publicKeyMain[20];
            uint8_t publicKeyAppend;
            uint8_t privateKeyMain[20];
            uint8_t privateKeyAppend;
        };
    };
    uint32_t seed1;
    uint32_t seed2;
    bool m_usePrivateKey;

public:
#ifndef QT_CORE_LIB
    Key(const char *publicKey = NULL, const char *privateKey = NULL);
#else
    Key(const QString &publicKey = QString(), const QString &privateKey = QString());
#endif

    inline void setUsePrivateKey(const bool &usePrivateKey) { m_usePrivateKey = usePrivateKey; }

    void resetSeed(void);

    void randSeed(void);

    uint8_t nextValue(void);

    void updatePublicKey(const char *publicKey);

    void updatePrivateKey(const char *privateKey);
};

class KeyCompute
{
public:
    union JDE_WORKSPACE_BLOCK
    {
        uint8_t c[64];
        uint32_t l[16];
    };

private:
    uint32_t m_state[5];
    uint32_t m_count[2];
    uint8_t m_buffer[64];
    uint8_t m_digest[20];

    uint8_t m_workspace[64];
    JDE_WORKSPACE_BLOCK *m_block; // JDE pointer to the byte array above

public:
    KeyCompute(void);

    void reset(void);

    void update(uint8_t *data, uint32_t len);

    void final(void);

    static void computeJde(const char *key, char *target);

private:
    void transform(uint32_t *state, uint8_t *buffer);
};

#pragma pack(pop)

}

#endif//__JdeKey_h__
