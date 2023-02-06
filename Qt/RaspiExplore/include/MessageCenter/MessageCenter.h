#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>

using std::ostringstream;

class MessageCenter : public std::basic_ostream<char> {
public:
    static MessageCenter& Instance();
    std::string HandleLastMessage();
    std::string HandleMessages();
    void SendMessage(const std::string &message);
    std::string GetMessages();
    // 处理 << endl
    MessageCenter& operator << (std::basic_ostream<char>& (*__pf)(std::basic_ostream<char>&));
    // 处理 << std::string/int/double ...
    template<typename T>
    MessageCenter& operator << (T elem)
    {
        if (m_messages.empty()) {
            m_messages.push_back("");
        }
        auto& lastMsg = *(m_messages.rbegin());

        std::ostringstream msg(std::ios_base::ate);
        msg << elem;

        lastMsg += msg.str();
        std::cout << msg.str();

        return *this;
    }

    // 处理vector的流
    template<typename T>
    MessageCenter& operator << (std::vector<T> elemList)
    {
        std::ostringstream msg(std::ios_base::ate);
        msg << "[";
        std::for_each(elemList.begin(), elemList.end(), [&msg](T elem) { msg << elem << " "; });
        msg << "]";

        SendMessage(msg.str());

        return *this;
    }

    void RefreshMessageUntill(const std::function<std::string()>& msgGenerator, const std::function<bool()>& isStopRefresh, int refreshPeriod = 2);
private:
    MessageCenter();
    ~MessageCenter() = default;

    static MessageCenter* m_msgCenter;
    std::vector<std::string> m_messages;
};