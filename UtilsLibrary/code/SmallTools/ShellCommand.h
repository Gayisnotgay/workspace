#pragma once
#include <string>
#include <stdio.h>
#include <memory>
#include "MessageCenter/MessageCenter.h"

class ShellCommand {
public:
    static std::string Exec(const std::string& cmd)
    {
        auto fdResult = popen(cmd.c_str(), "r");
        if (fdResult == nullptr) {
            MessageCenter::Instance() << "ShellCommand Exec failed due to invalid file descriptor, cmd:\"" << cmd << std::endl;
            return std::string("");
        }

        constexpr unsigned long maxResultBuffSize = 4096;
        auto resultBuff = std::make_unique<char[]>(maxResultBuffSize);
        auto realReadCount = fread(resultBuff.get(), 1, maxResultBuffSize - 1, fdResult);
        if (realReadCount >= maxResultBuffSize) {
            MessageCenter::Instance() << "ShellCommand Exec failed due to invalid read count, cmd:\"" << cmd << std::endl;
            return std::string("");
        }
        resultBuff[realReadCount] = '\0';
        pclose(fdResult);
        return std::string(resultBuff.get());
    }
};