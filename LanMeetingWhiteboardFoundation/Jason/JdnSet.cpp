#include "JdnSet.h"

using namespace Jdn;

Set::Set(void):
    m_begin(NULL),
    m_end(NULL),
    m_nodeStored(NULL),
    m_dataStored(NULL),
    m_jdeKey(NULL),
    m_privateKeyFlag(-1),
    m_callbackOnPrivateKeyFind(NULL),
    m_size(0),
    m_readyNext(true)
{ }

Set::Set(const Set &other):
    m_begin(NULL),
    m_end(NULL),
    m_nodeStored(NULL),
    m_dataStored(NULL),
    m_jdeKey(NULL),
    m_privateKeyFlag(-1),
    m_callbackOnPrivateKeyFind(NULL),
    m_size(0),
    m_readyNext(true)
{
    (*this) += other;
}

#if (__cplusplus >= 201103)
Set::Set(const std::initializer_list<Jdn::Node> &nodes):
    Set()
{
    for(const auto &now: nodes)
    {
        this->push_back(now);
    }
}
#endif

Set::~Set(void)
{
    this->clear();
    if(m_nodeStored)
    {
        free(m_nodeStored);
    }
    if(m_dataStored)
    {
        free(m_dataStored);
    }
    if(m_jdeKey)
    {
        delete m_jdeKey;
    }
}

Set &Set::operator+=(const Set &other)
{
    Node *now = other.m_begin;
    while(now)
    {
        this->push_back(*now);
        now = now->nextNode;
    }
    return *this;
}

Set &Set::operator=(const Set &other)
{
    this->clear();
    (*this) += other;
    return *this;
}

void Set::setJdeDisabled(void)
{
    if(m_jdeKey)
    {
        delete m_jdeKey;
        m_jdeKey = NULL;
    }
}

void Set::setJdeKey(const char *publicKey, const char *privateKey)
{
    if(publicKey)
    {
        m_jdeKey = new Jde::Key(publicKey, privateKey);
    }
    else if(m_jdeKey)
    {
        delete m_jdeKey;
        m_jdeKey = NULL;
    }
}

void Set::setJdeKey(const Jde::Key &key)
{
    if(m_jdeKey) { delete m_jdeKey; }
    m_jdeKey = new Jde::Key(key);
}

#if (__cplusplus >= 201103)
void Set::setCallbackOnPrivateKeyFind(const std::function<void (Node *, Set *)> &callBackOnPrivateKeyFind, const int32_t &privateKeyFlag)
{
    if(m_callbackOnPrivateKeyFind) { delete m_callbackOnPrivateKeyFind; }

    m_callbackOnPrivateKeyFind = new std::function<void (Node *, Set *)>(callBackOnPrivateKeyFind);
    m_privateKeyFlag = privateKeyFlag;
}
#else
void Set::setCallbackOnPrivateKeyFind(void (*callBackOnPrivateKeyFind)(Node *, Set *), const int32_t &privateKeyFlag)
{
    m_callbackOnPrivateKeyFind = callBackOnPrivateKeyFind;
    m_privateKeyFlag = privateKeyFlag;
}
#endif

void Set::push_back(const int32_t &num, const bool &useValidation)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);
    this->push_back(Node::createOnMemory(target, num, useValidation, nodeMemoryOwner));
}

void Set::push_back(const char *string, const bool &useValidation)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    void *buf;
    int size = strlen(string);
    bool dataMemoryOwner;
    this->newDataMemory(size + 1, buf, dataMemoryOwner);

    memcpy(buf, string, size);
    ((char *)buf)[size] = '\0';

    this->push_back(Node::createOnMemory(target, (const char *)buf, size, useValidation, nodeMemoryOwner, dataMemoryOwner, false));
}

void Set::push_back(const void *byteArray, const int32_t &size, const bool &useValidation)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    void *buf;
    bool dataMemoryOwner;
    this->newDataMemory(size, buf, dataMemoryOwner);

    memcpy(buf, byteArray, size);

    this->push_back(Node::createOnMemory(target, buf, size, useValidation, nodeMemoryOwner, dataMemoryOwner, false));
}

void Set::push_back(const int32_t &num, const int32_t &intData, const bool &useValidation)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);
    this->push_back(Node::createOnMemory(target, num, intData, useValidation, nodeMemoryOwner));
}

void Set::push_back(const int32_t &num, const char *string, const bool &useValidation)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    void *buf;
    int size = strlen(string);
    bool dataMemoryOwner;
    this->newDataMemory(size + 1, buf, dataMemoryOwner);

    memcpy(buf, string, size);
    ((char *)buf)[size] = '\0';

    this->push_back(Node::createOnMemory(target, num, (const char *)buf, size, useValidation, nodeMemoryOwner, dataMemoryOwner, false));
}

void Set::push_back(const int32_t &num, const void *byteArray, const int32_t &size, const bool &useValidation)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    void *buf;
    bool dataMemoryOwner;
    this->newDataMemory(size, buf, dataMemoryOwner);

    memcpy(buf, byteArray, size);

    this->push_back(Node::createOnMemory(target, num, buf, size, useValidation, nodeMemoryOwner, dataMemoryOwner, false));
}

void Set::push_back(const Node &other)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    this->push_back(Node::createOnMemory(target, &other, nodeMemoryOwner));
}

#ifdef QT_CORE_LIB
void Set::push_back(const QString &string_, const bool &useValidation)
{
    const auto &&string = string_.toUtf8();

    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    void *buf;
    bool dataMemoryOwner;
    this->newDataMemory(string.size() + 1, buf, dataMemoryOwner);

    memcpy(buf, string.data(), string.size());
    ((char *)buf)[string.size()] = '\0';

    this->push_back(Node::createOnMemory(target, (const char *)buf, string.size(), useValidation, nodeMemoryOwner, dataMemoryOwner, false));
}

void Set::push_back(const QByteArray &byteArray, const bool &useValidation)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    void *buf;
    bool dataMemoryOwner;
    this->newDataMemory(byteArray.size(), buf, dataMemoryOwner);

    memcpy(buf, byteArray.data(), byteArray.size());

    this->push_back(Node::createOnMemory(target, buf, byteArray.size(), useValidation, nodeMemoryOwner, dataMemoryOwner, false));
}

void Set::push_back(const int32_t &num, const QString &string_, const bool &useValidation)
{
    const auto &&string = string_.toUtf8();

    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    void *buf;
    bool dataMemoryOwner;
    this->newDataMemory(string.size() + 1, buf, dataMemoryOwner);

    memcpy(buf, string.data(), string.size());
    ((char *)buf)[string.size()] = '\0';

    this->push_back(Node::createOnMemory(target, num, (const char *)buf, string.size(), useValidation, nodeMemoryOwner, dataMemoryOwner, false));
}

void Set::push_back(const int32_t &num, const QByteArray &byteArray, const bool &useValidation)
{
    Node *target;
    bool nodeMemoryOwner;
    this->newNodeMemory(target, nodeMemoryOwner);

    void *buf;
    bool dataMemoryOwner;
    this->newDataMemory(byteArray.size(), buf, dataMemoryOwner);

    memcpy(buf, byteArray.data(), byteArray.size());

    this->push_back(Node::createOnMemory(target, num, buf, byteArray.size(), useValidation, nodeMemoryOwner, dataMemoryOwner, false));
}

void Set::push_back_variant(const QVariant &data, const bool &useValidation)
{
    switch(data.type())
    {
        case QVariant::Int: { this->push_back(data.toInt(), useValidation); break; }
        case QVariant::String: { this->push_back(data.toString(), useValidation); break; }
        case QVariant::ByteArray: { this->push_back(data.toByteArray(), useValidation); break; }
        default: { this->push_back(data.toString().toLatin1().data(), useValidation); break; }
    }
}
#endif

bool Set::isEmpty(void) const
{
    return (m_begin == m_end) && !m_begin;
}

int Set::size(void) const
{
    return m_size;
}

bool Set::pop_front(void)
{
    if(m_begin)
    {
        m_size--;
        Node *buf = m_begin->nextNode;
        m_begin->deleteNode();
        m_begin = buf;
    }

    if(!m_begin)
    {
        m_end = NULL;
    }

    return !this->isEmpty();
}

void Set::clear(void)
{
    this->readyNext();
    m_size = 0;

    while(m_begin)
    {
        m_end = m_begin->nextNode;
        m_begin->deleteNode();
        m_begin = m_end;
    }
}

void Set::readyNext(void)
{
    m_readyNext = true;
}

bool Set::next(void)
{
    if(m_readyNext)
    {
        m_readyNext = false;
        return m_begin;
    }
    else
    {
        this->pop_front();
        return !this->isEmpty();
    }
}

int Set::toStreamConservativeSize(void) const
{
    int conservativeSize = 0;
    Node *now = m_begin;
    while(now)
    {
        conservativeSize += now->toStreamConservativeSize();
        now = now->nextNode;
    }
    return conservativeSize;
}

int Set::toStream(void *target)
{
    m_size = 0;

    if(m_jdeKey)
    {
        m_jdeKey->resetSeed();
    }

    uint8_t *now = (uint8_t *)target;
    while(m_begin)
    {
        now += m_begin->toStream(now, m_jdeKey);

        if((m_begin->num == m_privateKeyFlag) && m_callbackOnPrivateKeyFind)
        {
            (*m_callbackOnPrivateKeyFind)(m_begin, this);
        }

        m_end = m_begin->nextNode;
        m_begin->deleteNode();
        m_begin = m_end;
    }
    return now - (uint8_t *)target;
}

int Set::toStreamWithSizeFlag(void *target)
{
    const uint32_t size = this->toStream((void *)((uint8_t *)target + 4));
    *(uint32_t *)target = size;

    return size + 4;
}

bool Set::fromStream(const void *source, const int &size, const int &faultTolerant_)
{
    this->clear();

    if(size)
    {
        uint8_t const *now = (uint8_t const *)source;
        const uint8_t *end = now + size;
        int buf, faultTolerant = faultTolerant_;

        if(m_jdeKey)
        {
            m_jdeKey->resetSeed();
        }

        while(now < end)
        {
            if(*now & NodeStreamStartFlag)
            {
                buf = Node::fromStream(now, end - now, this, m_jdeKey);
                if(buf > 0)
                {
                    now += buf;

                    if((m_end->num == m_privateKeyFlag) && m_callbackOnPrivateKeyFind)
                    {
                        (*m_callbackOnPrivateKeyFind)(m_end, this);
                    }

                    continue;
                }
                else
                {
                    if(!(faultTolerant--))
                    {
                        this->clear();
                        return false;
                    }
                }
            }
            now++;
        }
    }

    return true;
}

bool Set::fromStreamWithSizeFlag(const void *source, const int &size, const int &faultTolerant)
{
    switch(Set::checkStreamSize(source, size))
    {
        case StreamSizeJustFine:
        {
            return this->fromStream((const void *)((const char *)source + 4), size - 4, faultTolerant);
        }
        default:
        {
            return false;
        }
    }
}

StreamSizeFlag Set::checkStreamSize(const void *source, const int &size)
{
    if(size < 4) { return StreamSizeNeedWaitMore; }

    const uint32_t targetSize = *(const uint32_t *)source;
    if((size > JdnStreamMaxSize) || (targetSize > JdnStreamMaxSize)) { return StreamSizeOverMaxSize; }

    if(targetSize > ((uint32_t)size - 4)) { return StreamSizeNeedWaitMore; }

    if(targetSize < ((uint32_t)size - 4)) { return StreamSizeOverTargetSize; }

    return StreamSizeJustFine;
}

int Set::checkStreamTargetSize(const void *source, const int &size)
{
    if(size < 4) { return 0; }

    const uint32_t targetSize = *(const uint32_t *)source;
    return targetSize;
}

#ifdef QT_CORE_LIB
QByteArray Set::toStream(void)
{
    void *&&buf = malloc(this->toStreamConservativeSize());
    const auto &&size = this->toStream(buf);

    QByteArray buf2((const char *)buf, size);

    free(buf);
    return buf2;
}

QByteArray Set::toStreamWithSizeFlag(void)
{
    void *&&buf = malloc(this->toStreamConservativeSize() + 4);
    const auto &&size = this->toStream((void *)((uint8_t *)buf + 4));

    *(uint32_t *)buf = size;
    QByteArray buf2((const char *)buf, size + 4);

    free(buf);
    return buf2;
}

bool Set::fromStream(const QByteArray &source, const int &faultTolerant)
{
    return this->fromStream((const void *)source.data(), source.size(), faultTolerant);
}

bool Set::fromStreamWithSizeFlag(const QByteArray &source, const int &faultTolerant)
{
    return this->fromStreamWithSizeFlag((const void *)source.data(), source.size(), faultTolerant);
}

StreamSizeFlag Set::checkStreamSize(const QByteArray &source)
{
    return Set::checkStreamSize((const void *)source.data(), source.size());
}

int Set::checkStreamTargetSize(const QByteArray &source)
{
    return Set::checkStreamTargetSize((const void *)source.data(), source.size());
}
#endif

void Set::newNodeMemory(Node *&target, bool &isMemoryOwner)
{
    if(!m_nodeStored)
    {
        if(m_dataStored)
        {
            int size = *(int *)m_dataStored / sizeof(Node);
            m_nodeStored = (Node *)m_dataStored;
            *(int *)m_nodeStored = size;
            m_dataStored = NULL;
        }
        else
        {
            int size = this->size() * 1.5 + 2;
            if(size > JdnSetNodeMemoryMaxSize) { size = JdnSetNodeMemoryMaxSize; }

            m_nodeStored = (Node *)malloc(sizeof(Node) * size);
            *(int *)m_nodeStored = size;
        }
    }

    if(*(int *)m_nodeStored == 1)
    {
        target = m_nodeStored;
        isMemoryOwner = true;
        m_nodeStored = NULL;
    }
    else
    {
        target = m_nodeStored + (--(*(int *)m_nodeStored));
        isMemoryOwner = false;
    }
}

void Set::newDataMemory(const unsigned int &size, void *&target, bool &isMemoryOwner)
{
    if(size < JdnSetDataMemoryMaxSize)
    {
        if(!m_dataStored)
        {
            int size_ = size * 2 + sizeof(Node);
            if(size_ < JdnSetDataMemoryMinSize) { size_ = JdnSetDataMemoryMinSize; }

            m_dataStored = malloc(size_);
            *(int *)m_dataStored = size_;
        }

        if((*(int *)m_dataStored - sizeof(Node)) >= size)
        {
            *(int *)m_dataStored -= size;
            target = (uint8_t *)m_dataStored + *(int *)m_dataStored;
            isMemoryOwner = false;
        }
        else
        {
            target = malloc(size);
            isMemoryOwner = true;
        }
    }
    else
    {
        target = malloc(size);
        isMemoryOwner = true;
    }
}

void Set::returnDataMemory(const int &size)
{
    *(int *)m_dataStored += size;
}

void Set::push_back(Node *node)
{
    m_size++;

    if(m_end)
    {
        m_end->nextNode = node;
        m_end = node;
    }
    else
    {
        m_begin = m_end = node;
    }
}
