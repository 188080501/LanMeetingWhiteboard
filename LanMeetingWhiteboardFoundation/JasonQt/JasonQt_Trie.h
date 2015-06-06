/*
    JasonQt_Trie

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

#ifndef __JasonQt_Trie_h__
#define __JasonQt_Trie_h__

#define JasonQt_TrieDisabledFlagNum 0x0fffffff

// Qt lib import
#include <QString>
#include <QVector>
#include <QHash>
#include <QFile>
#include <QDebug>

#include <QtConcurrent>

namespace JasonQt_Trie
{

struct FlagData
{
    int flag;
    int level;

    FlagData(const int &flag = JasonQt_TrieDisabledFlagNum, const int &level = 0);
};

struct Node
{
    FlagData data;
    QHash<ushort, Node *> child;
};

class Dictionary
{
private:
    QVector<int> m_source;
    Node m_rootNode;

public:
    ~Dictionary(void);

    bool loadFromFile(const QString &filePath, const bool &onNewThread = true);

    void insrt(const QString &text, const int &flag, const int &level = 0);

    std::pair<bool, int> find(const QString &text) const;

    std::pair<bool, QMap<int, QVector<int> > > findAllFlag(const QString &text) const;

    void clear(void);

private:
    void deleteNode(Node *node);
};

class Seeker
{
private:
    int m_charCount = 1;
    Node *m_node;

public:
    Seeker(Node *node);

    std::pair<bool, std::pair<FlagData, int> > next(const ushort &c);
};

}

#endif//__JasonQt_Trie_h__
