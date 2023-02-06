#pragma once

#include <string>

class MyClassInterface {
public:
    virtual std::string GetData() const = 0;
    virtual void DataElementAdd(int add) = 0;
};

class MyClassWithOnlyCopy : public MyClassInterface {
    
public:
    MyClassWithOnlyCopy();

    explicit MyClassWithOnlyCopy(int dataSize, int* data);

    MyClassWithOnlyCopy(const MyClassWithOnlyCopy& other);

    MyClassWithOnlyCopy& operator=(const MyClassWithOnlyCopy& other);

    virtual ~MyClassWithOnlyCopy();

    std::string GetData() const override;

    void DataElementAdd(int add) override;

private:
    int m_dataSize = 0;
    int* m_data = nullptr;
};

class MyClassWithCopyAndMove : public MyClassInterface {
public:
    MyClassWithCopyAndMove();

    explicit MyClassWithCopyAndMove(int dataSize, int* data);

    MyClassWithCopyAndMove(const MyClassWithCopyAndMove& other);

    MyClassWithCopyAndMove(MyClassWithCopyAndMove&& other);

    MyClassWithCopyAndMove& operator=(const MyClassWithCopyAndMove& other);

    MyClassWithCopyAndMove& operator=(MyClassWithCopyAndMove&& other);

    virtual ~MyClassWithCopyAndMove();

    std::string GetData() const override;

    void DataElementAdd(int add) override;
private:
    int m_dataSize = 0;
    int* m_data = nullptr;
};

class MyClassWrapper {
public:
    MyClassWrapper() = default;
    virtual ~MyClassWrapper() = default;
    
    template<typename MyClass>
    std::string ShowWithPerfectForwarding(MyClass&& object)
    {
        typename std::remove_reference<MyClass>::type newObject(std::forward<MyClass>(object));
        newObject.DataElementAdd(125);

        return newObject.GetData();
    }

    std::string ShowWithRightReferrence(MyClassWithCopyAndMove&& object)
    {
        MyClassWithCopyAndMove newObject(std::move(object));
        newObject.DataElementAdd(125);

        return newObject.GetData();
    }

    std::string ShowWithLeftReferrence(MyClassWithCopyAndMove& object)
    {
        MyClassWithCopyAndMove newObject(object);
        newObject.DataElementAdd(125);

        return newObject.GetData();
    }
};