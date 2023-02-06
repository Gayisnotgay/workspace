#pragma once
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <tuple>
#include <sstream>
#include "AutoInitRuntimeObject/AutoInitRuntimeObject.h"
#include "MessageCenter/MessageCenter.h"

template<typename ReturnType, typename... ArgType>
class FuncFactory final {
public:
    using FuncType = std::function<ReturnType(ArgType... args)>;

    static FuncFactory& Instance()
    {
        static FuncFactory ins;
        return ins;
    }

    void RegFunc(const std::string& funcInfo, const FuncType& func)
    {
        GetFuncList().emplace(funcInfo, func);
    }

    std::tuple<int, std::string> GetAllFuncInfo()
    {
        std::ostringstream funcInfo;
        int funIdx = 0;
        std::for_each(GetFuncList().begin(), GetFuncList().end(), [&funcInfo,&funIdx](auto& func) { funcInfo << (funIdx++) << "." << func.first << std::endl; });

        return make_tuple(funIdx, funcInfo.str());
    }

    ReturnType RunFunc(const std::string& funcInfo, ArgType... paras)
    {
        auto patternIt = GetFuncList().find(funcInfo);
        if (patternIt == GetFuncList().end()) {
            MessageCenter::Instance() << funcInfo << " is unknown" << std::endl;
            exit(-1);
        }

        return patternIt->second(paras...);
    }

private:
    static std::map<std::string, FuncType>& GetFuncList()
    {
        static std::map<std::string, FuncType> funcList;
        return funcList;
    }
};

#ifndef FUNC_AUTO_REG
#define FUNC_AUTO_REG(funcIdentifier, funcInfo, func, paraTypes...) \
    volatile AutoInitRuntimeObject g_##funcIdentifier([](){ FuncFactory<paraTypes>::Instance().RegFunc(funcInfo, func); })
#endif