#include "TerminalCtrl.h"
#include <sstream>

using std::string;
using std::ostringstream;

#ifndef IMPL_TERMINALCTRL_FUNC 
#define IMPL_TERMINALCTRL_FUNC(Func, Str) \
string TerminalCtrl::Func() \
{\
    return string(Str);\
}
#endif

// 恢复默认配置
IMPL_TERMINALCTRL_FUNC(Reset,                    "\033[0m")
// 高亮
IMPL_TERMINALCTRL_FUNC(Highlight,                "\033[1m")
//闪烁
IMPL_TERMINALCTRL_FUNC(Shine,                    "\033[5m") 
//快闪    
IMPL_TERMINALCTRL_FUNC(QuickShine,               "\033[6m")
//反显     
IMPL_TERMINALCTRL_FUNC(ColorReverse,             "\033[7m")
// 消隐，消失隐藏 
IMPL_TERMINALCTRL_FUNC(Hide,                     "\033[8m") 
// 删除线
IMPL_TERMINALCTRL_FUNC(Strikeout,                "\033[9m") 
// 清屏
IMPL_TERMINALCTRL_FUNC(ClearScreen,              "\033[2J")
// 删除光标后面的当前行
IMPL_TERMINALCTRL_FUNC(ClearFromCursorToLineEnd, "\033[K")
// 保存光标当前位置
IMPL_TERMINALCTRL_FUNC(ReserveCursorPos,         "\033[s")
// 恢复光标到先前保存的位置
IMPL_TERMINALCTRL_FUNC(RestoreCursorPos,         "\033[u")
// 隐藏光标
IMPL_TERMINALCTRL_FUNC(HideCursor,               "\33[?25l")
// 显示光标
IMPL_TERMINALCTRL_FUNC(ShowCursor,               "\033[?25h")

// 设置前景色
string TerminalCtrl::FontColor(TerminalCtrl::Color color)
{
    ostringstream os("\033[", std::ios_base::ate);
    os << (30 + static_cast<int>(color)) << "m";
    return os.str();
}

// 设置背景色
string TerminalCtrl::BackGroundColor(TerminalCtrl::Color color)
{
    ostringstream os("\033[", std::ios_base::ate);
    os << (40 + static_cast<int>(color)) << "m";
    return os.str();
}

// 光标上移 
string TerminalCtrl::CursorUp(int offset)
{
    ostringstream os("\033[", std::ios_base::ate);
    os << offset << "A";
    return os.str();
}

// 光标下移 
string TerminalCtrl::CursorDown(int offset)
{
    ostringstream os("\033[", std::ios_base::ate);
    os << offset << "B";
    return os.str();
}

// 光标左移 
string TerminalCtrl::CursorLeft(int offset)
{
    ostringstream os("\033[", std::ios_base::ate);
    os << offset << "D";
    return os.str();
}

// 光标下右移 
string TerminalCtrl::CursorRight(int offset)
{
    ostringstream os("\033[", std::ios_base::ate);
    os << offset << "C";
    return os.str();
}

