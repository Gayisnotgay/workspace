#include "StdMoveHelperClass.h"
#include <algorithm>
#include <sstream>
#include <cstring>
#include "MessageCenter/MessageCenter.h"
#include "SmallTools/HelperDefine.h"

using std::for_each;
using std::string;
using std::ostringstream;
using std::memcpy;

MyClassWithOnlyCopy::MyClassWithOnlyCopy()
{
    MessageCenter::Instance().SendMessage("MyClassWithOnlyCopy Constructor: default;\n");
}

MyClassWithOnlyCopy::MyClassWithOnlyCopy(int dataSize, int* data)
{
    MessageCenter::Instance().SendMessage("MyClassWithOnlyCopy Constructor: custom;\n");

    m_dataSize = dataSize;
    m_data = new int[m_dataSize];
    memcpy(m_data, data, sizeof(int) * m_dataSize);
}

MyClassWithOnlyCopy::MyClassWithOnlyCopy(const MyClassWithOnlyCopy& other)
{
    MessageCenter::Instance().SendMessage("MyClassWithOnlyCopy Constructor: copy;\n");

    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }

    m_dataSize = other.m_dataSize;
    m_data = new int[m_dataSize];
    memcpy(m_data, other.m_data, sizeof(int) * m_dataSize);
}

MyClassWithOnlyCopy& MyClassWithOnlyCopy::operator=(const MyClassWithOnlyCopy& other)
{
    MessageCenter::Instance().SendMessage("MyClassWithOnlyCopy Assignment: copy;\n");

    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }

    m_dataSize = other.m_dataSize;
    m_data = new int[m_dataSize];
    memcpy(m_data, other.m_data, sizeof(int) * m_dataSize);

    return *this;
}

MyClassWithOnlyCopy::~MyClassWithOnlyCopy()
{
    MessageCenter::Instance().SendMessage("MyClassWithOnlyCopy Destructor: custom;\n");

    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }
}

string MyClassWithOnlyCopy::GetData() const
{
    ostringstream os(std::ios_base::ate);
    for_each(m_data, m_data + m_dataSize, [this, &os](int data) { os << data << " "; });

    return os.str();
}

void MyClassWithOnlyCopy::DataElementAdd(int add)
{
    for_each(m_data, m_data + m_dataSize, [&add](int& data) { data += add; });
}

MyClassWithCopyAndMove::MyClassWithCopyAndMove()
{
    MessageCenter::Instance().SendMessage("MyClassWithCopyAndMove Constructor: default;\n");
}

MyClassWithCopyAndMove::MyClassWithCopyAndMove(int dataSize, int* data)
{
    MessageCenter::Instance().SendMessage("MyClassWithCopyAndMove Constructor: custom;\n");

    m_dataSize = dataSize;
    m_data = new int[m_dataSize];
    memcpy(m_data, data, sizeof(int) * m_dataSize);
}

MyClassWithCopyAndMove::MyClassWithCopyAndMove(const MyClassWithCopyAndMove& other)
{
    MessageCenter::Instance().SendMessage("MyClassWithCopyAndMove Constructor: copy;\n");

    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }

    m_dataSize = other.m_dataSize;
    m_data = new int[m_dataSize];
    memcpy(m_data, other.m_data, sizeof(int) * m_dataSize);
}

MyClassWithCopyAndMove::MyClassWithCopyAndMove(MyClassWithCopyAndMove&& other)
{
    MessageCenter::Instance().SendMessage("MyClassWithCopyAndMove Constructor: move;\n");

    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }

    m_dataSize = other.m_dataSize;
    m_data = other.m_data;

    other.m_dataSize = 0;
    other.m_data = nullptr;
}

MyClassWithCopyAndMove& MyClassWithCopyAndMove::operator=(const MyClassWithCopyAndMove& other)
{
    MessageCenter::Instance().SendMessage("MyClassWithCopyAndMove Assignment: copy;\n");

    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }

    m_dataSize = other.m_dataSize;
    m_data = new int[m_dataSize];
    memcpy(m_data, other.m_data, sizeof(int) * m_dataSize);

    return *this;
}

MyClassWithCopyAndMove& MyClassWithCopyAndMove::operator=(MyClassWithCopyAndMove&& other)
{
    MessageCenter::Instance().SendMessage("MyClassWithCopyAndMove Assignment: move;\n");

    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }

    m_dataSize = other.m_dataSize;
    m_data = other.m_data;

    other.m_dataSize = 0;
    other.m_data = nullptr;

    return *this;
}

MyClassWithCopyAndMove::~MyClassWithCopyAndMove()
{
    MessageCenter::Instance().SendMessage("MyClassWithCopyAndMove Destructor: custom;\n");

    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }
}

string MyClassWithCopyAndMove::GetData() const
{
    ostringstream os(std::ios_base::ate);
    for_each(m_data, m_data + m_dataSize, [this, &os](int data) { os << data << " "; });

    return os.str();
}

void MyClassWithCopyAndMove::DataElementAdd(int add)
{
    for_each(m_data, m_data + m_dataSize, [&add](int& data) { data += add; });
}