#include "JdeKey.h"

using namespace Jde;

// Rotate x bits to the left
#define JDEROL32(_val32, _nBits) (((_val32)<<(_nBits))|((_val32)>>(32-(_nBits))))

#define JDEBLK0(i) (m_block->l[i] = (JDEROL32(m_block->l[i],24) & 0xFF00FF00) | (JDEROL32(m_block->l[i],8) & 0x00FF00FF))

#define JDEBLK(i) (m_block->l[i&15] = JDEROL32(m_block->l[(i+13)&15] ^ m_block->l[(i+8)&15] ^ m_block->l[(i+2)&15] ^ m_block->l[i&15],1))

// Jde rounds
#define JDER0(v,w,x,y,z,i) { z+=((w&(x^y))^y)+JDEBLK0(i)+0x5A827999+JDEROL32(v,5); w=JDEROL32(w,30); }

#define JDER1(v,w,x,y,z,i) { z+=((w&(x^y))^y)+JDEBLK(i)+0x5A827999+JDEROL32(v,5); w=JDEROL32(w,30); }

#define JDER2(v,w,x,y,z,i) { z+=(w^x^y)+JDEBLK(i)+0x6ED9EBA1+JDEROL32(v,5); w=JDEROL32(w,30); }

#define JDER3(v,w,x,y,z,i) { z+=(((w|x)&y)|(w&x))+JDEBLK(i)+0x8F1BBCDC+JDEROL32(v,5); w=JDEROL32(w,30); }

#define JDER4(v,w,x,y,z,i) { z+=(w^x^y)+JDEBLK(i)+0xCA62C1D6+JDEROL32(v,5); w=JDEROL32(w,30); }

// KeySource
#ifndef QT_CORE_LIB
Key::Key(const char *publicKey, const char *privateKey)
{
    this->updatePublicKey(publicKey);
    this->updatePrivateKey(privateKey);
}
#else
Key::Key(const QString &publicKey, const QString &privateKey)
{
    this->updatePublicKey((publicKey.isEmpty()) ? (NULL) : (publicKey.toLatin1().data()));
    this->updatePrivateKey((privateKey.isEmpty()) ? (NULL) : (privateKey.toLatin1().data()));
}
#endif

void Key::resetSeed(void)
{
    uint32_t *buf = (uint32_t *)(publicKeyMain + 17);
    seed1 = *buf;
    seed2 = 0;
}

void Key::randSeed(void)
{
    seed1 += ((seed2 & (seed1 ^ seed2)) ^ seed1) + 0x5A827999 + JDEROL32(seed2, 5);
    seed2 += ((seed1 & (seed2 ^ seed1)) ^ seed2) + 0x5A827999 + JDEROL32(seed1, 5);
}

uint8_t Key::nextValue(void)
{
    this->randSeed();
    return allData[((seed1 ^ seed2) + 0x6ED9EBA1) % ((m_usePrivateKey) ? (42) : (21))];
}

void Key::updatePublicKey(const char *publicKey)
{
    if(publicKey)
    {
        KeyCompute::computeJde(publicKey, (char *)publicKeyMain);
        uint32_t *buf = (uint32_t *)(publicKeyMain + 17);
        seed1 = *buf;
        publicKeyAppend = strlen(publicKey) % 256;
    }
    else
    {
        memset(publicKeyMain, '\0', 21);
        seed1 = 0;
    }
}

void Key::updatePrivateKey(const char *privateKey)
{
    if(privateKey)
    {
        KeyCompute::computeJde(privateKey, (char *)privateKeyMain);
        privateKeyAppend = strlen(privateKey) % 256;
        m_usePrivateKey = true;
    }
    else
    {
        memset(privateKeyMain, '\0', 21);
        m_usePrivateKey = false;
    }
    seed2 = 0;
}

// KeyCompute
KeyCompute::KeyCompute(void)
{
    m_block = (JDE_WORKSPACE_BLOCK *)m_workspace;
    reset();
}

void KeyCompute::reset(void)
{
    m_state[0] = 0x67452301;
    m_state[1] = 0xEFCDAB89;
    m_state[2] = 0x98BADCFE;
    m_state[3] = 0x10325476;
    m_state[4] = 0xC3D2E1F0;

    m_count[0] = 0;
    m_count[1] = 0;
}

void KeyCompute::update(uint8_t *data, uint32_t len)
{
    uint32_t i, j;

    j = (m_count[0] >> 3) & 63;

    if((m_count[0] += len << 3) < (len << 3))
    {
        m_count[1]++;
    }

    m_count[1] += (len >> 29);

    if((j + len) > 63)
    {
        i = 64 - j;
        memcpy(&m_buffer[j], data, i);
        transform(m_state, m_buffer);

        for(; i + 63 < len; i += 64)
        {
            transform(m_state, &data[i]);
        }

        j = 0;
    }
    else
    {
        i = 0;
    }

    memcpy(&m_buffer[j], &data[i], len - i);
}

void KeyCompute::final(void)
{
    uint32_t i;
    uint8_t finalcount[8];

    for(i = 0; i < 8; i++)
    {
        finalcount[i] = (uint8_t)((m_count[((i >= 4) ? 0 : 1)] >> ((3 - (i & 3)) * 8) ) & 255);
    }

    update((uint8_t *)"\200", 1);

    while((m_count[0] & 504) != 448)
    {
        update((uint8_t *)"\0", 1);
    }

    update(finalcount, 8);

    for(i = 0; i < 20; i++)
    {
        m_digest[i] = (uint8_t)((m_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
    }

    i = 0;
    memset(m_buffer, 0, 64);
    memset(m_state, 0, 20);
    memset(m_count, 0, 8);
    memset(finalcount, 0, 8);
    transform(m_state, m_buffer);
}

void KeyCompute::computeJde(const char *key, char *target)
{
    KeyCompute jde;
    jde.update((uint8_t *)key, strlen(key));
    jde.final();
    memcpy(target, jde.m_digest, 20);
}

void KeyCompute::transform(uint32_t *state, uint8_t *buffer)
{
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

    memcpy(m_block, buffer, 64);

    // 4 rounds of 20 operations each. Loop unrolled.
    JDER0(a,b,c,d,e, 0); JDER0(e,a,b,c,d, 1); JDER0(d,e,a,b,c, 2); JDER0(c,d,e,a,b, 3);
    JDER0(b,c,d,e,a, 4); JDER0(a,b,c,d,e, 5); JDER0(e,a,b,c,d, 6); JDER0(d,e,a,b,c, 7);
    JDER0(c,d,e,a,b, 8); JDER0(b,c,d,e,a, 9); JDER0(a,b,c,d,e,10); JDER0(e,a,b,c,d,11);
    JDER0(d,e,a,b,c,12); JDER0(c,d,e,a,b,13); JDER0(b,c,d,e,a,14); JDER0(a,b,c,d,e,15);
    JDER1(e,a,b,c,d,16); JDER1(d,e,a,b,c,17); JDER1(c,d,e,a,b,18); JDER1(b,c,d,e,a,19);
    JDER2(a,b,c,d,e,20); JDER2(e,a,b,c,d,21); JDER2(d,e,a,b,c,22); JDER2(c,d,e,a,b,23);
    JDER2(b,c,d,e,a,24); JDER2(a,b,c,d,e,25); JDER2(e,a,b,c,d,26); JDER2(d,e,a,b,c,27);
    JDER2(c,d,e,a,b,28); JDER2(b,c,d,e,a,29); JDER2(a,b,c,d,e,30); JDER2(e,a,b,c,d,31);
    JDER2(d,e,a,b,c,32); JDER2(c,d,e,a,b,33); JDER2(b,c,d,e,a,34); JDER2(a,b,c,d,e,35);
    JDER2(e,a,b,c,d,36); JDER2(d,e,a,b,c,37); JDER2(c,d,e,a,b,38); JDER2(b,c,d,e,a,39);
    JDER3(a,b,c,d,e,40); JDER3(e,a,b,c,d,41); JDER3(d,e,a,b,c,42); JDER3(c,d,e,a,b,43);
    JDER3(b,c,d,e,a,44); JDER3(a,b,c,d,e,45); JDER3(e,a,b,c,d,46); JDER3(d,e,a,b,c,47);
    JDER3(c,d,e,a,b,48); JDER3(b,c,d,e,a,49); JDER3(a,b,c,d,e,50); JDER3(e,a,b,c,d,51);
    JDER3(d,e,a,b,c,52); JDER3(c,d,e,a,b,53); JDER3(b,c,d,e,a,54); JDER3(a,b,c,d,e,55);
    JDER3(e,a,b,c,d,56); JDER3(d,e,a,b,c,57); JDER3(c,d,e,a,b,58); JDER3(b,c,d,e,a,59);
    JDER4(a,b,c,d,e,60); JDER4(e,a,b,c,d,61); JDER4(d,e,a,b,c,62); JDER4(c,d,e,a,b,63);
    JDER4(b,c,d,e,a,64); JDER4(a,b,c,d,e,65); JDER4(e,a,b,c,d,66); JDER4(d,e,a,b,c,67);
    JDER4(c,d,e,a,b,68); JDER4(b,c,d,e,a,69); JDER4(a,b,c,d,e,70); JDER4(e,a,b,c,d,71);
    JDER4(d,e,a,b,c,72); JDER4(c,d,e,a,b,73); JDER4(b,c,d,e,a,74); JDER4(a,b,c,d,e,75);
    JDER4(e,a,b,c,d,76); JDER4(d,e,a,b,c,77); JDER4(c,d,e,a,b,78); JDER4(b,c,d,e,a,79);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;

    a = b = c = d = e = 0;
}
