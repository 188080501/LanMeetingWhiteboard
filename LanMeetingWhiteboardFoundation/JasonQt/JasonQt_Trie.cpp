#include "JasonQt_Trie.h"

using namespace JasonQt_Trie;

// FlagData
FlagData::FlagData(const int &flag_, const int &level_):
    flag(flag_),
    level(level_)
{ }

// Dictionary
Dictionary::~Dictionary(void)
{
    this->clear();
}

bool Dictionary::loadFromFile(const QString &filePath, const bool &onNewThread)
{
    if(onNewThread)
    {
        QtConcurrent::run([=]()
        {
            this->loadFromFile(filePath, false);
        });

        return true;
    }
    else
    {
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly)) { return false; }

        int lineCount = 0;
        while(true)
        {
            lineCount++;

            if(file.atEnd()) { break; }

            QByteArray data(file.readLine());
            if(data.isEmpty() || (data == "\n")) { continue; }

            QTextStream stream(data);
            int level = JasonQt_TrieDisabledFlagNum, flag = JasonQt_TrieDisabledFlagNum;
            QString text;

            stream >> level >> flag >> text;
            if(text.isEmpty() || (level == JasonQt_TrieDisabledFlagNum) || (flag == JasonQt_TrieDisabledFlagNum))
            {
                qDebug() << "Dictionary::readFromFile Warning: On line" << lineCount << "read error";
                continue;
            }

            this->insrt(text, flag, level);
        }

        return true;
    }
}

void Dictionary::insrt(const QString &text, const int &flag, const int &level)
{
    Node *node = &m_rootNode, *buf;
    m_source.push_back(flag);

    for(int index = 0; index < text.size(); index++)
    {
        const auto &&unicode = text.at(index).unicode();
        const auto &&it = node->child.find(unicode);

        if(it == node->child.end())
        {
            buf = new Node;
            node->child.insert(unicode, buf);
        }
        else
        {
            buf = it.value();
        }

        if((index + 1) == text.size())
        {
            buf->data.flag = m_source.size() - 1;
            buf->data.level = level;
        }

        node = buf;
    }
}

std::pair<bool, int> Dictionary::find(const QString &text) const
{
    QList<Seeker> seekers;
    int answerFlag = JasonQt_TrieDisabledFlagNum, answerCharCount = 0, answerLevel = INT_MIN;

    for(int index = 0; index < text.size(); index++)
    {
        const auto &&unicode = text.at(index).unicode();

        QList<Seeker> buf;
        for(auto &seeker: seekers)
        {
            const auto &&answer = seeker.next(unicode);
            if(answer.first)
            {
                buf.push_back(seeker);

                if((answer.second.first.flag != JasonQt_TrieDisabledFlagNum) && (((answer.second.first.level == answerLevel) && (answer.second.second > answerCharCount))
                                                                               || (answer.second.first.level > answerLevel)))
                {
                    answerFlag = answer.second.first.flag;
                    answerCharCount = answer.second.second;
                    answerLevel = answer.second.first.level;
                }
            }
        }
        seekers = buf;

        const auto &&it = m_rootNode.child.find(unicode);

        if(it != m_rootNode.child.end())
        {
            seekers.push_back( { it.value() } );

            if((it.value()->data.flag != JasonQt_TrieDisabledFlagNum) && ((answerFlag == JasonQt_TrieDisabledFlagNum) || (it.value()->data.level > answerLevel)))
            {
                answerFlag = it.value()->data.flag;
                answerCharCount = 1;
                answerLevel = it.value()->data.level;
            }
        }
    }

    if(answerFlag == JasonQt_TrieDisabledFlagNum) { return { false, JasonQt_TrieDisabledFlagNum }; }

    return { true, m_source.at(answerFlag) };
}

std::pair<bool, QMap<int, QVector<int> > > Dictionary::findAllFlag(const QString &text) const
{
    QList<Seeker> seekers;
    QMap<int, QVector<int>> answerSum;

    for(int index = 0; index < text.size(); index++)
    {
        const auto &&unicode = text.at(index).unicode();

        QList<Seeker> buf;
        for(auto &seeker: seekers)
        {
            const auto &&answer = seeker.next(unicode);
            if(answer.first)
            {
                buf.push_back(seeker);

                if(answer.second.first.flag != JasonQt_TrieDisabledFlagNum)
                {
                    answerSum[answer.second.first.level].push_back(m_source[answer.second.first.flag]);
                }
            }
        }
        seekers = buf;

        const auto &&it = m_rootNode.child.find(unicode);

        if(it != m_rootNode.child.end())
        {
            seekers.push_back( { it.value() } );

            if(it.value()->data.flag != JasonQt_TrieDisabledFlagNum)
            {
                answerSum[it.value()->data.level].push_back(m_source[it.value()->data.flag]);
            }
        }
    }

    return { !answerSum.isEmpty(), answerSum };
}

void Dictionary::clear(void)
{
    for(auto &now: m_rootNode.child)
    {
        this->deleteNode(now);
    }
}

void Dictionary::deleteNode(Node *node)
{
    for(auto &now: node->child)
    {
        this->deleteNode(now);
    }
    delete node;
}

// Seeker
Seeker::Seeker(Node *node):
    m_node(node)
{ }

std::pair<bool, std::pair<FlagData, int> > Seeker::next(const ushort &c)
{
    const auto &&it = m_node->child.find(c);

    if(it == m_node->child.end()) { return { false, { { JasonQt_TrieDisabledFlagNum, 0 }, 0 } }; }
    m_node = it.value();

    return { true, { { it.value()->data }, ++m_charCount } };
}
