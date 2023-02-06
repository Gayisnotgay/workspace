#pragma once
#include <string>

class TerminalCtrl {
public:
    enum class Color : int {
        BLACK = 0,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        PURPLE,
        DRAKGREEN,
        WHITE
    };

    // 恢复默认配置
    static std::string Reset();
    // 高亮
    static std::string Highlight();
    // 设置前景色
    static std::string FontColor(Color color);
    // 设置背景色
    static std::string BackGroundColor(Color color);
    //闪烁
    static std::string Shine();  
    //快闪    
    static std::string QuickShine();
    // 删除线
    static std::string Strikeout();  
    //反显     
    static std::string ColorReverse();   
    // 消隐，消失隐藏 
    static std::string Hide();    
    // 清屏
    static std::string ClearScreen();
    // 删除光标后面的当前行
    static std::string ClearFromCursorToLineEnd();
    // 光标上移 
    static std::string CursorUp(int offset);
    // 光标下移 
    static std::string CursorDown(int offset);
    // 光标左移 
    static std::string CursorLeft(int offset);
    // 光标下右移 
    static std::string CursorRight(int offset);
    // 保存光标当前位置
    static std::string ReserveCursorPos();
    // 恢复光标到先前保存的位置
    static std::string RestoreCursorPos();
    // 隐藏光标
    static std::string HideCursor();
    // 显示光标
    static std::string ShowCursor();
};
