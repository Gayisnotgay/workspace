#pragma once

#include <functional>

using InitFunc = std::function<void()>;

class AutoInitRuntimeObject {
public:
    AutoInitRuntimeObject(const InitFunc& init);
    virtual ~AutoInitRuntimeObject() = default;
};