#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <algorithm>
#include <tuple>

class StringAlign {
public:
    // <格式化后的字符串, 列宽>
    static std::string OutputTableRow(const std::vector<std::string>& row, size_t columnWidth = std::numeric_limits<size_t>::max())
    {
        size_t width = columnWidth;
        if (width == std::numeric_limits<size_t>::max()) {
            width = 0;
            std::for_each(row.begin(), row.end(), [&width](const auto& elem){ width = (elem.length() > width) ? elem.length() : width; });
            // column之间要有空格
            width += 2;
        }

        std::ostringstream formatStr(std::ios_base::ate);
        formatStr << std::setiosflags(std::ios_base::left);
        std::for_each(row.begin(), row.end(), [&formatStr, &width](const auto& elem){ formatStr << std::setw(width) << elem; });

        return formatStr.str();
    }
};