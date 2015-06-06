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

#ifndef __JdnNode_h__
#define __JdnNode_h__

// Jdn lib import
#include "JdnDefine.h"

namespace Jdn
{

class Set;

#pragma pack(push)
#pragma pack(1)

class Node
{
public:
    int32_t num;
    int32_t intData;
    Node *nextNode;
    void *data;

private:
    uint8_t m_nodeFlag;

public:
    Node(const int32_t &num, const bool &useValidation = false, const bool &nodeMemoryOwner = true);

    Node(const char *string, const int32_t &size = -1, const bool &useValidation = false, const bool &nodeMemoryOwner = true, const bool &dataMemoryOwner = true, const bool &duplicate = true);

    Node(const void *byteArray, const int32_t &size, const bool &useValidation = false, const bool &nodeMemoryOwner = true, const bool &dataMemoryOwner = true, const bool &duplicate = true);

    Node(const int32_t &num, const int32_t &intData, const bool &useValidation = false, const bool &nodeMemoryOwner = true);

    Node(const int32_t &num, const char *string, const int32_t &size = -1, const bool &useValidation = false, const bool &nodeMemoryOwner = true, const bool &dataMemoryOwner = true, const bool &duplicate = true);

    Node(const int32_t &num, const void *byteArray, const int32_t &size, const bool &useValidation = false, const bool &nodeMemoryOwner = true, const bool &dataMemoryOwner = true, const bool &duplicate = true);

    Node(const Node &other);

#if (__cplusplus >= 201103)
    template<typename PackageType>
    Node(const PackageType &package);

    Node &operator=(const Node &) = delete;
#endif

    static Node *createOnMemory(Node *target, const int32_t &num, const bool &useValidation, const bool &nodeMemoryOwner);

    static Node *createOnMemory(Node *target, const char *string, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate);

    static Node *createOnMemory(Node *target, const void *byteArray, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate);

    static Node *createOnMemory(Node *target, const int32_t &num, const int32_t &intData, const bool &useValidation, const bool &nodeMemoryOwner);

    static Node *createOnMemory(Node *target, const int32_t &num, const char *string, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate);

    static Node *createOnMemory(Node *target, const int32_t &num, const void *byteArray, const int32_t &size, const bool &useValidation, const bool &nodeMemoryOwner, const bool &dataMemoryOwner, const bool &duplicate);

    static Node *createOnMemory(Node *target, const Node *source, const bool &nodeMemoryOwner);


    void deleteNode(void);


    int toStreamConservativeSize(void) const;

    int toStream(void *target, Jde::Key *jdeKey);

    static int fromStream(const void *source, const int &size, Set *set, Jde::Key *jdeKey);

    static uint8_t computeValidation(const void *source, const int &size);


    inline bool isUseValidation(void) const { return m_nodeFlag & UseValidationFlag; }

    inline bool isNodeMemoryOwner(void) const { return m_nodeFlag & NodeMemoryOwnerFlag; }

    inline bool isDataMemoryOwner(void) const { return m_nodeFlag & DataMemoryOwnerFlag; }

    inline NodeType nodeType(void) const { return NodeType(m_nodeFlag & NodeTypeRange); }
};

#if (__cplusplus >= 201103)
template<typename PackageType>
Node::Node(const PackageType &package):
    Node(PackageType::packageNum(), (const void *)&package, sizeof(PackageType), PackageType::packageIsUseValidation())
{ }
#endif

#pragma pack(pop)

}

#endif//__JdnNode_h__
