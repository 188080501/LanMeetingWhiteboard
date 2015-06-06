#include "JdnNode.h"
#include "JdnSet.h"

using namespace Jdn;

Node::Node(const int32_t &num, const bool &useValidation, const bool &nodeMemoryOwner)
{
    Node::createOnMemory(this, num, useValidation, nodeMemoryOwner);
}

Node::Node(const char *string, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate)
{
    Node::createOnMemory(this, string, size, useValidation, nodeMemoryOwner, dataMemoryOwner, duplicate);
}

Node::Node(const void *byteArray, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate)
{
    Node::createOnMemory(this, byteArray, size, useValidation, nodeMemoryOwner, dataMemoryOwner, duplicate);
}

Node::Node(const int32_t &num, const int32_t &intData, const bool &useValidation, const bool &nodeMemoryOwner)
{
    Node::createOnMemory(this, num, intData, useValidation, nodeMemoryOwner);
}

Node::Node(const int32_t &num, const char *string, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate)
{
    Node::createOnMemory(this, num, string, size, useValidation, nodeMemoryOwner, dataMemoryOwner, duplicate);
}

Node::Node(const int32_t &num, const void *byteArray, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate)
{
    Node::createOnMemory(this, num, byteArray, size, useValidation, nodeMemoryOwner, dataMemoryOwner, duplicate);
}

Node::Node(const Node &other)
{
    Node::createOnMemory(this, &other, true);
}

Node *Node::createOnMemory(Node *target, const int32_t &num_, const bool &useValidation, const bool &nodeMemoryOwner)
{
    target->num = num_;
    target->intData = 0;
    target->nextNode = NULL;
    target->data = NULL;
    target->m_nodeFlag = OnlyNumNode;

    if(useValidation) { target->m_nodeFlag |= UseValidationFlag; }
    if(nodeMemoryOwner) { target->m_nodeFlag |= NodeMemoryOwnerFlag; }
    target->m_nodeFlag |= DataMemoryOwnerFlag;

    return target;
}

Node *Node::createOnMemory(Node *target, const char *string, const int32_t &size_, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate)
{
    target->num = 0;
    target->nextNode = NULL;
    target->m_nodeFlag = OnlyStringNode;

    if(useValidation) { target->m_nodeFlag |= UseValidationFlag; }
    if(nodeMemoryOwner) { target->m_nodeFlag |= NodeMemoryOwnerFlag; }
    if(dataMemoryOwner) { target->m_nodeFlag |= DataMemoryOwnerFlag; }

    target->intData = (size_ == -1) ? (strlen(string)) : (size_);

    if(duplicate)
    {
        target->data = malloc(target->intData + 1);
        ((char *)target->data)[target->intData] = '\0';
        memcpy(target->data, string, target->intData);
    }
    else
    {
        target->data = (void *)string;
    }

    return target;
}

Node *Node::createOnMemory(Node *target, const void *byteArray, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate)
{
    target->num = 0;
    target->intData = size;
    target->nextNode = NULL;
    target->m_nodeFlag = OnlyByteArrayNode;

    if(useValidation) { target->m_nodeFlag |= UseValidationFlag; }
    if(nodeMemoryOwner) { target->m_nodeFlag |= NodeMemoryOwnerFlag; }
    if(dataMemoryOwner) { target->m_nodeFlag |= DataMemoryOwnerFlag; }

    if(duplicate)
    {
        target->data = malloc(target->intData);
        memcpy(target->data, byteArray, target->intData);
    }
    else
    {
        target->data = (void *)byteArray;
    }

    return target;
}

Node *Node::createOnMemory(Node *target, const int32_t &num_, const int32_t &intData_, const bool &useValidation, const bool &nodeMemoryOwner)
{
    target->num = num_;
    target->intData = intData_;
    target->nextNode = NULL;
    target->data = NULL;
    target->m_nodeFlag = WithIntNode;

    if(useValidation) { target->m_nodeFlag |= UseValidationFlag; }
    if(nodeMemoryOwner) { target->m_nodeFlag |= NodeMemoryOwnerFlag; }
    target->m_nodeFlag |= DataMemoryOwnerFlag;

    return target;
}

Node *Node::createOnMemory(Node *target, const int32_t &num_, const char *string, const int32_t &size_, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate)
{
    target->num = num_;
    target->nextNode = NULL;
    target->m_nodeFlag = WithStringNode;

    if(useValidation) { target->m_nodeFlag |= UseValidationFlag; }
    if(nodeMemoryOwner) { target->m_nodeFlag |= NodeMemoryOwnerFlag; }
    if(dataMemoryOwner) { target->m_nodeFlag |= DataMemoryOwnerFlag; }

    target->intData = (size_ == -1) ? (strlen(string)) : (size_);

    if(duplicate)
    {
        target->data = malloc(target->intData + 1);
        ((char *)target->data)[target->intData] = '\0';
        memcpy(target->data, string, target->intData);
    }
    else
    {
        target->data = (void *)string;
    }

    return target;
}

Node *Node::createOnMemory(Node *target, const int32_t &num_, const void *byteArray, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate)
{
    target->num = num_;
    target->intData = size;
    target->nextNode = NULL;
    target->m_nodeFlag = WithByteArrayNode;

    if(useValidation) { target->m_nodeFlag |= UseValidationFlag; }
    if(nodeMemoryOwner) { target->m_nodeFlag |= NodeMemoryOwnerFlag; }
    if(dataMemoryOwner) { target->m_nodeFlag |= DataMemoryOwnerFlag; }

    if(duplicate)
    {
        target->data = malloc(target->intData);
        memcpy(target->data, byteArray, target->intData);
    }
    else
    {
        target->data = (void *)byteArray;
    }

    return target;
}

Node *Node::createOnMemory(Node *target, const Node *source, const bool &nodeMemoryOwner)
{
    target->num = source->num;
    target->intData = source->intData;
    target->nextNode = NULL;

    target->m_nodeFlag = source->nodeType();

    if(source->isUseValidation()) { target->m_nodeFlag |= UseValidationFlag; }
    if(nodeMemoryOwner) { target->m_nodeFlag |= NodeMemoryOwnerFlag; }

    switch(source->nodeType())
    {
        case Jdn::OnlyStringNode:
        case Jdn::WithStringNode:
        {
            target->m_nodeFlag |= DataMemoryOwnerFlag;

            target->data = malloc(target->intData + 1);
            ((char *)target->data)[target->intData] = '\0';
            memcpy(target->data, source->data, target->intData);
            break;
        }
        case Jdn::OnlyByteArrayNode:
        case Jdn::WithByteArrayNode:
        {
            target->m_nodeFlag |= DataMemoryOwnerFlag;

            target->data = malloc(target->intData);
            memcpy(target->data, source->data, target->intData);
            break;
        }
        default:
        {
            target->data = NULL;
            break;
        }
    }

    return target;
}

void Node::deleteNode(void)
{
    if(data && this->isDataMemoryOwner())
    {
        free(data);
    }

    if(this->isNodeMemoryOwner())
    {
        free(this);
    }
}

int Node::toStreamConservativeSize(void) const
{
    switch(this->nodeType())
    {
        case OnlyNumNode:       { return 1 + sizeof(num) + ((this->isUseValidation()) ? (1) : (0)); }
        case OnlyStringNode:
        case OnlyByteArrayNode: { return 1 + sizeof(intData) + intData + ((this->isUseValidation()) ? (1) : (0)); }
        case WithIntNode:       { return 1 + sizeof(num) + sizeof(intData) + ((this->isUseValidation()) ? (1) : (0)); }
        case WithStringNode:
        case WithByteArrayNode: { return 1 + sizeof(num) + sizeof(intData) + intData + ((this->isUseValidation()) ? (1) : (0)); }
        default: { return 0; }
    }
}

int Node::toStream(void *target, Jde::Key *jdeKey)
{
    uint8_t *now = (uint8_t *)target;

    *now = NodeStreamStartFlag | ((this->isUseValidation()) ? (NodeStreamUseValidationFlag) : (0)) | this->nodeType();
    now++;

    // Write Num
    switch(this->nodeType())
    {
        case OnlyNumNode:
        case WithIntNode:
        case WithStringNode:
        case WithByteArrayNode:
        {
            if(num & 0xffff0000)
            {
                Jde::encryption(&num, sizeof(num), now, jdeKey);
                now += sizeof(num);
            }
            else
            {
                *(uint8_t *)target |= NodeStreamNumCompressionFlag;
                Jde::encryption(&num, 2, now, jdeKey);
                now += 2;
            }
            break;
        }
        default: { break; }
    }

    // Write IntData
    switch(this->nodeType())
    {
        case OnlyStringNode:
        case OnlyByteArrayNode:
        case WithIntNode:
        case WithStringNode:
        case WithByteArrayNode:
        {
            if(intData & 0xffff0000)
            {
                Jde::encryption(&intData, sizeof(intData), now, jdeKey);
                now += sizeof(intData);
            }
            else
            {
                *(uint8_t *)target |= NodeStreamIntDataCompressionFlag;

                Jde::encryption(&intData, 2, now, jdeKey);
                now += 2;
            }
            break;
        }
        default: { break; }
    }

    // Write Data
    switch(this->nodeType())
    {
        case OnlyStringNode:
        case OnlyByteArrayNode:
        case WithStringNode:
        case WithByteArrayNode:
        {
            Jde::encryption(data, intData, now, jdeKey);
            now += intData;
            break;
        }
        default: { break; }
    }

    // Write Validation
    if(this->isUseValidation())
    {
        *(uint8_t *)target |= NodeStreamUseValidationFlag;

        switch(this->nodeType())
        {
            case OnlyNumNode:
            {
                *now = Node::computeValidation(&num, sizeof(num));
                if(jdeKey) { *now += jdeKey->nextValue(); }
                now++;
                break;
            }
            case OnlyStringNode:
            case OnlyByteArrayNode:
            {
                *now = Node::computeValidation(&intData, sizeof(intData));
                *now ^= Node::computeValidation(data, intData);
                if(jdeKey) { *now += jdeKey->nextValue(); }
                now++;
                break;
            }
            case WithIntNode:
            {
                *now = Node::computeValidation(&num, sizeof(num));
                *now ^= Node::computeValidation(&intData, sizeof(intData));
                if(jdeKey) { *now += jdeKey->nextValue(); }
                now++;
                break;
            }
            case WithStringNode:
            case WithByteArrayNode:
            {
                *now = Node::computeValidation(&num, sizeof(num));
                *now ^= Node::computeValidation(&intData, sizeof(intData));
                *now ^= Node::computeValidation(data, intData);
                if(jdeKey) { *now += jdeKey->nextValue(); }
                now++;
                break;
            }
            default: { break; }
        }
    }

    return now - (uint8_t *)target;
}

int Node::fromStream(const void *source, const int &size, Set *set, Jde::Key *jdeKey)
{
    uint8_t const *now = (uint8_t const *)source;
    int surplus = size - 1;
    uint8_t nodeFlag = *(now++);
    int32_t num = 0, intData = 0;

    if((nodeFlag & NodeTypeRange) == UnKnowNode) { return -1; }

    // Read Num
    switch(nodeFlag & NodeTypeRange)
    {
        case OnlyNumNode:
        case WithIntNode:
        case WithStringNode:
        case WithByteArrayNode:
        {
            if(nodeFlag & NodeStreamNumCompressionFlag)
            {
                if(surplus < 2) { return -1; }
                Jde::decrypt(now, 2, &num, jdeKey);
                now += 2;
            }
            else
            {
                if(surplus < 4) { return -1; }
                Jde::decrypt(now, sizeof(num), &num, jdeKey);
                now += 4;
            }
            break;
        }
    }

    // Read IntData
    switch(nodeFlag & NodeTypeRange)
    {
        case OnlyStringNode:
        case OnlyByteArrayNode:
        case WithIntNode:
        case WithStringNode:
        case WithByteArrayNode:
        {
            if(nodeFlag & NodeStreamIntDataCompressionFlag)
            {
                if(surplus < 2) { return -1; }
                Jde::decrypt(now, 2, &intData, jdeKey);
                now += 2;
            }
            else
            {
                if(surplus < 4) { return -1; }
                Jde::decrypt(now, sizeof(intData), &intData, jdeKey);
                now += 4;
            }
            break;
        }
    }

    // Read Data
    void *data = NULL;
    bool targetDataMemoryOwner;

    switch(nodeFlag & NodeTypeRange)
    {
        case OnlyStringNode:
        case OnlyByteArrayNode:
        case WithStringNode:
        case WithByteArrayNode:
        {
            if((intData > JdnNodeDataMaxSize) || (intData < 0)) { return -1; }

            if(surplus < intData) { return -1; }

            switch(nodeFlag & NodeTypeRange)
            {
                case OnlyStringNode:
                case WithStringNode:
                {
                    set->newDataMemory(intData + 1, data, targetDataMemoryOwner);
                    ((char *)data)[intData] = '\0';
                    break;
                }
                default:
                {
                    set->newDataMemory(intData, data, targetDataMemoryOwner);
                    break;
                }
            }
            Jde::decrypt(now, intData, data, jdeKey);
            now += intData;
            break;
        }
    }

    // Read Validation
    if(nodeFlag & NodeStreamUseValidationFlag)
    {
        uint8_t validation = 1;

        if(surplus > 0)
        {
            switch(nodeFlag & NodeTypeRange)
            {
                case OnlyNumNode:
                {
                    validation = Node::computeValidation(&num, sizeof(num));
                    if(jdeKey) { validation += jdeKey->nextValue(); }
                    validation = (*(now++) == validation);
                    break;
                }
                case OnlyStringNode:
                case OnlyByteArrayNode:
                {
                    validation = Node::computeValidation(&intData, sizeof(intData));
                    validation ^= Node::computeValidation(data, intData);
                    if(jdeKey) { validation += jdeKey->nextValue(); }
                    validation = (*(now++) == validation);
                    break;
                }
                case WithIntNode:
                {
                    validation = Node::computeValidation(&num, sizeof(num));
                    validation ^= Node::computeValidation(&intData, sizeof(intData));
                    if(jdeKey) { validation += jdeKey->nextValue(); }
                    validation = (*(now++) == validation);
                    break;
                }
                case WithStringNode:
                case WithByteArrayNode:
                {
                    validation = Node::computeValidation(&num, sizeof(num));
                    validation ^= Node::computeValidation(&intData, sizeof(intData));
                    validation ^= Node::computeValidation(data, intData);
                    if(jdeKey) { validation += jdeKey->nextValue(); }
                    validation = (*(now++) == validation);
                    break;
                }
                default: { break; }
            }
        }

        if(!validation)
        {
            if(targetDataMemoryOwner)
            {
                free(data);
            }
            else
            {
                switch(nodeFlag & NodeTypeRange)
                {
                    case OnlyStringNode:
                    case WithStringNode:
                    {
                        set->returnDataMemory(intData + 1);
                        break;
                    }
                    case OnlyByteArrayNode:
                    case WithByteArrayNode:
                    {
                        set->returnDataMemory(intData);
                        break;
                    }
                }
            }
            return -1;
        }
    }

    // Make Node
    Node *target;
    bool targetNodeMemoryOwner;
    set->newNodeMemory(target, targetNodeMemoryOwner);

    switch(nodeFlag & NodeTypeRange)
    {
        case OnlyNumNode:
        {
            set->push_back(Node::createOnMemory(target, num, bool(nodeFlag & NodeStreamUseValidationFlag), targetNodeMemoryOwner));
            return now - (uint8_t *)source;
        }
        case OnlyStringNode:
        {
            set->push_back(Node::createOnMemory(target, (const char *)data, intData, bool(*(uint8_t *)source & NodeStreamUseValidationFlag), targetNodeMemoryOwner, targetDataMemoryOwner, false));
            return now - (uint8_t *)source;
        }
        case OnlyByteArrayNode:
        {
            set->push_back(Node::createOnMemory(target, (const void *)data, intData, bool(*(uint8_t *)source & NodeStreamUseValidationFlag), targetNodeMemoryOwner, targetDataMemoryOwner, false));
            return now - (uint8_t *)source;
        }
        case WithIntNode:
        {
            set->push_back(Node::createOnMemory(target, num, intData, bool(*(uint8_t *)source & NodeStreamUseValidationFlag), targetNodeMemoryOwner));
            return now - (uint8_t *)source;
        }
        case WithStringNode:
        {
            set->push_back(Node::createOnMemory(target, num, (const char *)data, intData, bool(*(uint8_t *)source & NodeStreamUseValidationFlag), targetNodeMemoryOwner, targetDataMemoryOwner, false));
            return now - (uint8_t *)source;
        }
        case WithByteArrayNode:
        {
            set->push_back(Node::createOnMemory(target, num, (const void *)data, intData, bool(*(uint8_t *)source & NodeStreamUseValidationFlag), targetNodeMemoryOwner, targetDataMemoryOwner, false));
            return now - (uint8_t *)source;
        }
    }

    return -1;
}

uint8_t Node::computeValidation(const void *source, const int &size)
{
    uint8_t const *now = (uint8_t const *)source, *end = (uint8_t const *)source + size;
    uint8_t buf = 0;
    while(now < end)
    {
        buf ^= *now++;
    }
    return buf;
}
