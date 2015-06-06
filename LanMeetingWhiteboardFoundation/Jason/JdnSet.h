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

#ifndef __JdnSet_h__
#define __JdnSet_h__

// Jdn lib import
#include "JdnNode.h"

namespace Jdn
{

#ifdef QT_CORE_LIB
#   define JdnSingleDataAgreement(Num, UseValidation)                               \
                                                                                    \
    static inline constexpr int32_t packageNum(void) { return Num; }                \
                                                                                    \
    static inline constexpr bool packageIsUseValidation(void) { return UseValidation; }

#   define JdnMultiDataAgreement(Num, UseValidation, PackageDataCount)              \
                                                                                    \
    static inline constexpr int32_t packageNum(void) { return Num; }                \
                                                                                    \
    static inline constexpr bool packageIsUseValidation(void) { return UseValidation; } \
                                                                                    \
    static inline constexpr int packageDataCount(void) { return PackageDataCount; }
#else
#   define JdnSingleDataAgreement(Num, UseValidation)                               \
                                                                                    \
    static inline const int32_t packageNum(void) { return Num; }                    \
                                                                                    \
    static inline const bool packageIsUseValidation(void) { return UseValidation; }
#endif

class Set
{
    friend class Node;

private:
    Node *m_begin;
    Node *m_end;

    Node *m_nodeStored;
    void *m_dataStored;

    Jde::Key *m_jdeKey;
    int32_t m_privateKeyFlag;

#if (__cplusplus >= 201103)
    std::function< void(Jdn::Node *node, Jdn::Set *set) > *m_callbackOnPrivateKeyFind = NULL;
#else
    void (*m_callbackOnPrivateKeyFind)(Jdn::Node *node, Jdn::Set *set);
#endif

    int m_size;
    bool m_readyNext;

public:
    Set(void);

    Set(const Set &other);

#if (__cplusplus >= 201103)
    Set(const std::initializer_list<Jdn::Node> &nodes);
#endif

    ~Set(void);

    Set &operator+=(const Set &other);

    Set &operator=(const Set &other);


    inline Jde::Key *jdeKey(void) { return m_jdeKey; }

    void setJdeDisabled(void);

    void setJdeKey(const char *publicKey, const char *privateKey = NULL);

    void setJdeKey(const Jde::Key &key);

#if (__cplusplus >= 201103)
    void setCallbackOnPrivateKeyFind(const std::function< void(Jdn::Node *node, Jdn::Set *set) > &callBackOnPrivateKeyFind, const int32_t &privateKeyFlag);
#else
    void setCallbackOnPrivateKeyFind(void (*callBackOnPrivateKeyFind)(Node *, Set *), const int32_t &privateKeyFla);
#endif


    void push_back(const int32_t &num, const bool &useValidation = false);

    void push_back(const char *string, const bool &useValidation = false);

    void push_back(const void *byteArray, const int32_t &size, const bool &useValidation = false);

    void push_back(const int32_t &num, const int32_t &intData, const bool &useValidation = false);

    void push_back(const int32_t &num, const char *string, const bool &useValidation = false);

    void push_back(const int32_t &num, const void *byteArray, const int32_t &size, const bool &useValidation = false);

    void push_back(const Jdn::Node &other);

#ifdef QT_CORE_LIB
    void push_back(const QString &string, const bool &useValidation = false);

    void push_back(const QByteArray &byteArray, const bool &useValidation = false);

    void push_back(const int32_t &num, const QString &string, const bool &useValidation = false);

    void push_back(const int32_t &num, const QByteArray &byteArray, const bool &useValidation = false);

    void push_back_variant(const QVariant &data, const bool &useValidation = false);
#endif

    template<typename PackageType>
    void push_back_single_data(const PackageType &package);

#ifdef QT_CORE_LIB
    template<typename PackageType>
    void push_back_single_data(const QVector<PackageType> &packages);

    template<typename PackageType>
    void push_back_multi_data(const PackageType &package);

    template<typename PackageType>
    void push_back_multi_data(const QVector<PackageType> &packages);
#endif

    bool isEmpty(void) const;

    int size(void) const;

    bool pop_front(void);

    void clear(void);

    void readyNext(void);

    bool next(void);


    int toStreamConservativeSize(void) const;

    int toStream(void *target);

    int toStreamWithSizeFlag(void *target);

    bool fromStream(const void *source, const int &size, const int &faultTolerant = 0);

    bool fromStreamWithSizeFlag(const void *source, const int &size, const int &faultTolerant = 0);

    static StreamSizeFlag checkStreamSize(const void *source, const int &size);

    static int checkStreamTargetSize(const void *source, const int &size);

#ifdef QT_CORE_LIB
    QByteArray toStream(void);

    QByteArray toStreamWithSizeFlag(void);

    bool fromStream(const QByteArray &source, const int &faultTolerant = 0);

    bool fromStreamWithSizeFlag(const QByteArray &source, const int &faultTolerant = 0);

    static StreamSizeFlag checkStreamSize(const QByteArray &source);

    static int checkStreamTargetSize(const QByteArray &source);
#endif

    inline const Node *currentNode(void) const { return m_begin; }

    inline Jdn::NodeType currentNodeType(void) const { return m_begin->nodeType(); }

    inline bool currentNodeIsUseValidation(void) const { return m_begin->isUseValidation(); }

    inline int32_t currentNodeNum(void) const { return m_begin->num; }

    inline int32_t currentNodeIntData(void) const { return m_begin->intData; }

    inline int32_t currentNodeSize(void) const { return m_begin->intData; }

    inline const char *currentNodeString(void) const { return (const char *)m_begin->data; }

    inline const void *currentNodeByteArray(void) const { return m_begin->data; }

#ifdef QT_CORE_LIB
    inline const QString currentNodeQString(void) const { return QString((const char *)m_begin->data); }

    inline const QByteArray currentNodeQByteArray(void) const { return QByteArray((const char *)m_begin->data, m_begin->intData); }
#endif

    template<typename PackageType>
    bool copy_current_from_single_data(PackageType &target);

#ifdef QT_CORE_LIB
    template<typename PackageType>
    bool copy_current_from_multi_data(PackageType &target);
#endif

private:
    void newNodeMemory(Node *&target, bool &isMemoryOwner);

    void newDataMemory(const unsigned int &size, void *&target, bool &isMemoryOwner);

    void returnDataMemory(const int &size);

    void push_back(Node *node);
};

template<typename PackageType>
void Set::push_back_single_data(const PackageType &package)
{
    this->push_back(PackageType::packageNum(), &package, sizeof(PackageType), PackageType::packageIsUseValidation());
}

#ifdef QT_CORE_LIB
template<typename PackageType>
void Set::push_back_single_data(const QVector<PackageType> &packages)
{
    for(const auto &package: packages)
    {
        this->push_back_single_data(package);
    }
}

template<typename PackageType>
void Set::push_back_multi_data(const PackageType &package)
{
    QVariantList buf;

    package.toVariantList(buf);
    this->push_back(PackageType::packageNum(), PackageType::packageDataCount(), PackageType::packageIsUseValidation());

    for(const auto &now: buf)
    {
        this->push_back_variant(now, PackageType::packageIsUseValidation());
    }
}

template<typename PackageType>
void Set::push_back_multi_data(const QVector<PackageType> &packages)
{
    for(const auto &package: packages)
    {
        this->push_back_multi_data(package);
    }
}
#endif

template<typename PackageType>
bool Set::copy_current_from_single_data(PackageType &target)
{
    if(this->currentNodeType() != Jdn::WithByteArrayNode) { return false; }

    if(this->currentNodeNum() != PackageType::packageNum()) { return false; }

    if(this->currentNodeSize() != sizeof(PackageType)) { return false; }

    if(this->currentNodeIsUseValidation() != PackageType::packageIsUseValidation()) { return false; }

    memcpy(&target, this->currentNodeByteArray(), sizeof(PackageType));

    return true;
}

#ifdef QT_CORE_LIB
template<typename PackageType>
bool Set::copy_current_from_multi_data(PackageType &target)
{
    if(this->size() < (PackageType::packageDataCount() + 1)) { return false; }

    if(this->currentNodeType() != Jdn::WithIntNode) { return false; }

    if(this->currentNodeNum() != PackageType::packageNum()) { return false; }

    if(this->currentNodeIntData() != PackageType::packageDataCount()) { return false; }

    if(this->currentNodeIsUseValidation() != PackageType::packageIsUseValidation()) { return false; }

    QVariantList buf;
    for(int count = PackageType::packageDataCount(); count--; )
    {
        this->pop_front();

        if(this->currentNodeIsUseValidation() != PackageType::packageIsUseValidation()) { return false; }

        switch(this->currentNodeType())
        {
            case Jdn::OnlyNumNode: { buf.push_back(this->currentNodeNum()); break; }
            case Jdn::OnlyStringNode: { buf.push_back(this->currentNodeQString()); break; }
            case Jdn::OnlyByteArrayNode: { buf.push_back(this->currentNodeQByteArray()); break; }
            default: { return false; }
        }
    }

    this->pop_front();

    return target.fromVariantList(buf);
}
#endif

}

#endif//__JdnSet_h__
