#include "MessageCenter.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include "TerminalCtrl/TerminalCtrl.h"

using std::cout;
using std::endl;
using std::for_each;
using std::string;
using std::function;

MessageCenter* MessageCenter::m_msgCenter = nullptr;

MessageCenter& MessageCenter::Instance()
{
    if (m_msgCenter == nullptr) {
        m_msgCenter = new MessageCenter();
    }
    
    return *m_msgCenter;
}

string MessageCenter::GetMessages()
{
    ostringstream messages(std::ios_base::ate);
    for_each(m_messages.begin(), m_messages.end(), [&messages](const string &message) { messages << message; });

    return messages.str();
}

string MessageCenter::HandleMessages()
{
    auto messages = GetMessages();
    m_messages.clear();
    return messages;
}

string MessageCenter::HandleLastMessage()
{
    string message = m_messages.empty() ? " " : *(m_messages.rbegin());
    if (!m_messages.empty()) {
        m_messages.pop_back();
    }

    return message;
}

void MessageCenter::SendMessage(const string &message)
{
    m_messages.push_back(string(message));
    cout << message;
}

MessageCenter& MessageCenter::operator << (basic_ostream<char>& (*__pf)(basic_ostream<char>&))
{
    if (m_messages.empty()) {
        m_messages.push_back("");
    }
    auto& lastMsg = *(m_messages.rbegin());

    ostringstream msg(std::ios_base::ate);
    msg << __pf;

    lastMsg += msg.str();
    cout << __pf;

    return *this;
}

void MessageCenter::RefreshMessageUntill(const function<string()>& msgGenerator, const function<bool()>& isStopRefresh, int refreshPeriod)
{
    const string dynamicFlag("\\-/|");
    size_t flagOffset = 0;

    string preMsg;
    while (!isStopRefresh()) {
        cout << TerminalCtrl::CursorLeft(preMsg.length() + 1);
        preMsg = msgGenerator();
        cout << preMsg << dynamicFlag[flagOffset++];
        cout.flush();
        if (flagOffset >= dynamicFlag.length()) {
            flagOffset = 0;
        }
        sleep(refreshPeriod);
    }

    cout << endl;
    m_messages.push_back(preMsg);
}

MessageCenter::MessageCenter()
{
    m_messages.push_back("MessageCenter Created\n");
}