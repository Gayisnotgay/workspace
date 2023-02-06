#pragma once
#include <map>
#include <vector>
#include <string>

class ParseConfig {
public:
    static std::map<std::string, std::string> ParseAssignCfg(const std::string& cfg);
    // 按照逗号或者空格分隔
    static std::vector<std::string> SplitCfgStr(const std::string& cfg);
    // 按照输入的正则表达式循环解析
    static std::vector<std::string> ParsePattern(const std::string& cfg, const std::string& regPattern);
};