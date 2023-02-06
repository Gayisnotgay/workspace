#pragma once
#include "AutoRegFactory/AutoRegFactory.h"

class TerminalCtrlFunction {
public:
    TerminalCtrlFunction() = default;
    ~TerminalCtrlFunction() = default;
    int Run(int argc, char* argv[]);
};

#ifndef TERMINALCTRL_FUNC_AUTO_REG
#define TERMINALCTRL_FUNC_AUTO_REG(TerminalFunc) \
    FUNC_AUTO_REG(TerminalFunc, #TerminalFunc, [](int argc, char* argv[]){ return TerminalFunc().Run(argc, argv); }, int, int, char**)
#endif