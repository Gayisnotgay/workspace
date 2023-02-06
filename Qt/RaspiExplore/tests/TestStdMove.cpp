#include <string>
#include <sstream>
#include <functional>
#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MessageCenter/MessageCenter.h"
#include "StdMoveHelperClass.h"
#include "SmallTools/HelperDefine.h"

using std::string;
using std::ostringstream;
using std::function;
using namespace testing;

TEST(MyClassWithOnlyCopy, ShouldCallDefaultConstructorWhenDefineObject)
{
    MyClassWithOnlyCopy obj;
    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithOnlyCopy Constructor: default;\n");
}

TEST(MyClassWithOnlyCopy, ShouldCallCustomConstructorWhenDefineObject)
{
    int data[] = {100, 200};
    MyClassWithOnlyCopy obj(COUNTOF(data), data);
    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithOnlyCopy Constructor: custom;\n");
}

TEST(MyClassWithOnlyCopy, ShouldCallCopyConstructorWhenCallFunctionWithObjectPara)
{
    auto functor = [](MyClassWithOnlyCopy object) -> string {
        auto msg = MessageCenter::Instance().HandleLastMessage();
        EXPECT_THAT(msg, "MyClassWithOnlyCopy Constructor: copy;\n");
        return object.GetData();
    };

    int data[] = {100, 200};
    MyClassWithOnlyCopy obj(COUNTOF(data), data);
    auto strData = functor(obj);
    EXPECT_THAT(strData, "100 200 ");
}

TEST(MyClassWithOnlyCopy, ShouldOnlyCallCustomConstructorWhenCallFunctionWithObjectParaReferrence)
{
    auto functor = [](const MyClassWithOnlyCopy& object) -> string {
        auto msg = MessageCenter::Instance().HandleLastMessage();
        EXPECT_THAT(msg, "MyClassWithOnlyCopy Constructor: custom;\n");
        return object.GetData();
    };

    int data[] = {100, 200};
    MyClassWithOnlyCopy obj(COUNTOF(data), data);
    auto strData = functor(obj);
    EXPECT_THAT(strData, "100 200 ");
}

TEST(MyClassWithOnlyCopy, ShouldOnlyCallCustomConstructorWhenCallFunctionReturnWithObjectPara)
{
    auto functor = []() -> MyClassWithOnlyCopy {
        int data[] = {101, 2006};
        return MyClassWithOnlyCopy(COUNTOF(data), data);
    };

    EXPECT_THAT(functor().GetData(), "101 2006 ");
    auto object = functor();
    EXPECT_THAT(object.GetData(), "101 2006 ");

    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithOnlyCopy Constructor: custom;\n");
}

MyClassWithOnlyCopy CreateMyClassWithOnlyCopy()
{
    int data[] = {101, 2006};
    return MyClassWithOnlyCopy(COUNTOF(data), data);
};

TEST(MyClassWithOnlyCopy, ShouldOnlyCallCopyAssignmentWhenCallFunctionReturnWithObjectPara)
{
    EXPECT_THAT(CreateMyClassWithOnlyCopy().GetData(), "101 2006 ");

    MyClassWithOnlyCopy object1 = CreateMyClassWithOnlyCopy();
    EXPECT_THAT(object1.GetData(), "101 2006 ");

    int data[] = {103, 2060};
    MyClassWithOnlyCopy object2(COUNTOF(data), data);
    EXPECT_THAT(object2.GetData(), "103 2060 ");

    object2 = object1;
    EXPECT_THAT(object2.GetData(), "101 2006 ");

    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithOnlyCopy Assignment: copy;\n");
}

TEST(MyClassWithOnlyCopy, ShouldCallCustomConstructorAndCopyAssignmentWhenCallFunctionReturnWithObjectPara)
{
    auto functor = []() -> MyClassWithOnlyCopy {
        int data[] = {101, 2006};
        return MyClassWithOnlyCopy(COUNTOF(data), data);
    };

    MyClassWithOnlyCopy object;
    object = functor();
    EXPECT_THAT(object.GetData(), "101 2006 ");

    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithOnlyCopy Destructor: custom;\n");

    msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithOnlyCopy Assignment: copy;\n");
}

TEST(MyClassWithOnlyCopy, ShouldCallCustomConstructorWhenCallFunctionWithObjectParaRightReferrence)
{
    auto functor = [](const MyClassWithOnlyCopy&& object) -> string {
        auto msg = MessageCenter::Instance().HandleLastMessage();
        EXPECT_THAT(msg, "MyClassWithOnlyCopy Constructor: custom;\n");
        return object.GetData();
    };

    int data[] = {100, 200};
    MyClassWithOnlyCopy obj(COUNTOF(data), data);
    auto strData = functor(std::move(obj));
    EXPECT_THAT(strData, "100 200 ");
}

TEST(MyClassWithCopyAndMove, ShouldCallDefaultConstructorWhenDefineObject)
{
    MyClassWithCopyAndMove obj;
    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithCopyAndMove Constructor: default;\n");
}

TEST(MyClassWithCopyAndMove, ShouldCallCustomConstructorWhenDefineObject)
{
    int data[] = {100, 200};
    MyClassWithCopyAndMove obj(COUNTOF(data), data);
    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithCopyAndMove Constructor: custom;\n");
}

TEST(MyClassWithCopyAndMove, ShouldCallCopyConstructorWhenCallFunctionWithObjectPara)
{
    auto functor = [](MyClassWithCopyAndMove object) -> string {
        auto msg = MessageCenter::Instance().HandleLastMessage();
        EXPECT_THAT(msg, "MyClassWithCopyAndMove Constructor: copy;\n");
        return object.GetData();
    };

    int data[] = {100, 200};
    MyClassWithCopyAndMove obj(COUNTOF(data), data);
    auto strData = functor(obj);
    EXPECT_THAT(strData, "100 200 ");
}

TEST(MyClassWithCopyAndMove, ShouldOnlyCallCustomConstructorWhenCallFunctionWithObjectParaReferrence)
{
    auto functor = [](const MyClassWithCopyAndMove& object) -> string {
        auto msg = MessageCenter::Instance().HandleLastMessage();
        EXPECT_THAT(msg, "MyClassWithCopyAndMove Constructor: custom;\n");
        return object.GetData();
    };

    int data[] = {100, 200};
    MyClassWithCopyAndMove obj(COUNTOF(data), data);
    auto strData = functor(obj);
    EXPECT_THAT(strData, "100 200 ");
}

TEST(MyClassWithCopyAndMove, ShouldOnlyCallCustomConstructorWhenCallFunctionReturnWithObjectPara)
{
    auto functor = []() -> MyClassWithCopyAndMove {
        int data[] = {101, 2006};
        return MyClassWithCopyAndMove(COUNTOF(data), data);
    };

    auto object = functor();
    EXPECT_THAT(object.GetData(), "101 2006 ");

    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithCopyAndMove Constructor: custom;\n");
}

TEST(MyClassWithCopyAndMove, ShouldCallCustomConstructorAndMoveAssignmentWhenCallFunctionReturnWithObjectPara)
{
    (void)MessageCenter::Instance().HandleMessages();

    auto functor = []() -> MyClassWithCopyAndMove {
        int data[] = {101, 2006};
        return MyClassWithCopyAndMove(COUNTOF(data), data);
    };

    MyClassWithCopyAndMove object;
    auto&& ret_rreferrence = functor();
    object = ret_rreferrence;
    EXPECT_THAT(object.GetData(), "101 2006 ");

    auto msg = MessageCenter::Instance().HandleMessages();
    ostringstream expMsg(std::ios_base::ate);
    expMsg << "MyClassWithCopyAndMove Constructor: default;\n" \
           << "MyClassWithCopyAndMove Constructor: custom;\n" \
           << "MyClassWithCopyAndMove Assignment: copy;\n";
    EXPECT_THAT(msg, expMsg.str());
}

TEST(MyClassWithCopyAndMove, ShouldCallMoveConstructorWhenCallFunctionWithObjectParaRightReferrence)
{
    auto functor = [](MyClassWithCopyAndMove&& object) -> string {
        MyClassWithCopyAndMove tmpObject(std::move(object));
        auto msg = MessageCenter::Instance().HandleLastMessage();
        EXPECT_THAT(msg, "MyClassWithCopyAndMove Constructor: move;\n");
        return tmpObject.GetData();
    };

    int data[] = {100, 200};
    auto strData = functor(MyClassWithCopyAndMove(COUNTOF(data), data));
    EXPECT_THAT(strData, "100 200 ");
}

TEST(MyClassWithCopyAndMove, ShouldCallMoveConstructorWhenCallFunctionWhichReturnRReferrnce)
{
    MyClassWithCopyAndMove* object = nullptr;
    auto functor = [&object]() -> MyClassWithCopyAndMove&& {
        int data[] = {125, 260};
        object = new MyClassWithCopyAndMove(COUNTOF(data), data);
        return  std::move(*object);
    };

    auto newObject = functor();
    EXPECT_THAT(newObject.GetData(), "125 260 ");
    auto msg = MessageCenter::Instance().HandleLastMessage();
    EXPECT_THAT(msg, "MyClassWithCopyAndMove Constructor: move;\n"); 
    delete object;
}

TEST(MyClassWithCopyAndMove, JustTest)
{
    auto functor = [](MyClassWithCopyAndMove&& object) -> string {
        MyClassWithCopyAndMove newObject(std::move(object));
        auto msg = MessageCenter::Instance().HandleLastMessage();
        EXPECT_THAT(msg, "MyClassWithCopyAndMove Constructor: move;\n");
        return newObject.GetData();
    };

    int data[] = {100, 200};
    MyClassWithCopyAndMove obj(COUNTOF(data), data);
    auto strData = functor(std::move(obj));
    EXPECT_THAT(strData, "100 200 ");
    EXPECT_TRUE(obj.GetData().empty()) << obj.GetData();

    constexpr int maxStrSize = 100; 
    std::unique_ptr<char[]> customString = std::make_unique<char[]>(maxStrSize);
    memset(customString.get(), 0xc5, maxStrSize);
    std::unique_ptr<char[]> newString = std::make_unique<char[]>(maxStrSize);
    memset(newString.get(), 0x5c, maxStrSize);
    newString = std::move(customString);
    EXPECT_THAT(newString[99], 0xc5);
    EXPECT_FALSE(customString);

    MyClassWithOnlyCopy obj1(COUNTOF(data), data);
    MyClassWithOnlyCopy obj2 = std::move(obj1);
    EXPECT_THAT(obj1.GetData(), "100 200 ");
    EXPECT_THAT(obj2.GetData(), "100 200 ");
}

TEST(MyClassWrapper, ShowWithPerfectForwardingShouldCallCopyConstructorWhenPassingIntoLValue)
{
    (void)MessageCenter::Instance().HandleMessages();

    int data[] = {250, 250};
    MyClassWithCopyAndMove myClassObject(COUNTOF(data), data);
    EXPECT_THAT(myClassObject.GetData(), "250 250 ");
    auto dataInfo = MyClassWrapper().ShowWithPerfectForwarding(myClassObject);
    EXPECT_THAT(dataInfo, "375 375 ");

    auto msg = MessageCenter::Instance().HandleMessages();
    ostringstream expMsg(std::ios_base::ate);
    expMsg << "MyClassWithCopyAndMove Constructor: custom;\n" \
           << "MyClassWithCopyAndMove Constructor: copy;\n" \
           << "MyClassWithCopyAndMove Destructor: custom;\n";
    EXPECT_THAT(msg, expMsg.str());
}

TEST(MyClassWrapper, ShowWithPerfectForwardingShouldCallMoveConstructorWhenPassingIntoRValue)
{
    (void)MessageCenter::Instance().HandleMessages();

    int data[] = {250, 250};
    auto dataInfo = MyClassWrapper().ShowWithPerfectForwarding(MyClassWithCopyAndMove(COUNTOF(data), data));
    EXPECT_THAT(dataInfo, "375 375 ");

    auto msg = MessageCenter::Instance().HandleMessages();
    ostringstream expMsg(std::ios_base::ate);
    expMsg << "MyClassWithCopyAndMove Constructor: custom;\n" \
           << "MyClassWithCopyAndMove Constructor: move;\n" \
           << "MyClassWithCopyAndMove Destructor: custom;\n" \
           << "MyClassWithCopyAndMove Destructor: custom;\n";
    EXPECT_THAT(msg, expMsg.str());
}

TEST(MyClassWrapper, ShowWithRightReferrenceShouldCallMoveConstructorWhenPassingIntoRValue)
{
    (void)MessageCenter::Instance().HandleMessages();

    int data[] = {250, 250};
    auto dataInfo = MyClassWrapper().ShowWithRightReferrence(MyClassWithCopyAndMove(COUNTOF(data), data));
    EXPECT_THAT(dataInfo, "375 375 ");

    auto msg = MessageCenter::Instance().HandleMessages();
    ostringstream expMsg(std::ios_base::ate);
    expMsg << "MyClassWithCopyAndMove Constructor: custom;\n" \
           << "MyClassWithCopyAndMove Constructor: move;\n" \
           << "MyClassWithCopyAndMove Destructor: custom;\n" \
           << "MyClassWithCopyAndMove Destructor: custom;\n";
    EXPECT_THAT(msg, expMsg.str());
}