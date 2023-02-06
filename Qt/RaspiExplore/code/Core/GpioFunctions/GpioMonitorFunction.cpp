#include "GpioMonitorFunction.h"
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <functional>
#include <sstream>
#include <thread>
#include "ParseConfig/ParseConfig.h"
#include "SmallTools/StringAlign.h"
#include "SmallTools/TimeTool.h"
#include "MessageCenter/MessageCenter.h"
#include "TerminalCtrl/TerminalCtrl.h"

using std::string;
using std::stoul;
using std::cout;
using std::endl;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::get;
using std::for_each;
using std::function;
using std::ostringstream;
using std::bitset;
using std::thread;

int GpioMonitorFunction::Run(int argc, char* argv[])
{
	if (static_cast<size_t>(argc) != m_minParaLen + 1) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	vector<string> pins = ParseConfig::SplitCfgStr(argv[m_minParaLen]);
	if (pins.empty()) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	vector<string> args;
	for (size_t index = 0; index < m_minParaLen; ++index) {
		args.emplace_back(argv[index]);
	}

	args.insert(args.end(), pins.begin(), pins.end());

	string output;
	int result = Run(args, output, nullptr);
    if (result != EXIT_SUCCESS) {
		MessageCenter::Instance() << "GpioMonitorFunction failed: " << endl;
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}

// flag: 用来控制output数据同步，仅在GUI界面使用；命令行模式传入空指针
int GpioMonitorFunction::Run(const vector<string>& args, string& output, int* flag)
{
    if (GpioFunction::Run(args, output, flag) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

	GpioAccessParam param;
	if (ParseParam(vector<string>(args.begin() + m_minParaLen, args.end()), param) != EXIT_SUCCESS) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	// 为了避免GUI卡顿，需要创建子线程单独执行监控任务
	thread monitorThd([](GpioMonitorFunction* context, GpioAccessParam gpioParam, std::string& monitorOutput, int* ctrlFlag){ context->MonitorProcess(gpioParam, monitorOutput, ctrlFlag); }, this, param, std::ref(output), flag);

	// 命令行模式直接阻塞执行;
	// Gui模式则需要本函数尽快退出，不要阻塞用户界面
	if (flag == nullptr) {
		monitorThd.join();
	} else {
		monitorThd.detach();
	}

	return EXIT_SUCCESS;
}

void GpioMonitorFunction::MonitorProcess(GpioAccessParam param, string& output, int* flag)
{
	MessageCenter::Instance() << TerminalCtrl::Reset();

	// 打印表头
	vector<string> tblHead;
	for_each(get<0>(param).begin(), get<0>(param).end(), [&](int pin) { ostringstream os(std::ios_base::ate); os << "GPIO" << pin; tblHead.emplace_back(os.str()); });
	const size_t tableColumnLen = 15;
	MessageCenter::Instance() << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::WHITE) << TerminalCtrl::FontColor(TerminalCtrl::Color::BLACK) << StringAlign::OutputTableRow(tblHead, tableColumnLen) << endl;

	auto gpioMonitor = [this, &param, &output, &flag]()->string {
		// GpioMonitorFunction will stop refreshing [output] until caller reset this bit to b0
    	bitset<32> ctrlBitFlag((flag == nullptr) ? 0 : static_cast<unsigned long>(*flag));
		
		while (ctrlBitFlag[GpioMonitorFunction::MONITOR_SYNCHRONIZE_ENABLE] && ctrlBitFlag[GpioMonitorFunction::MONITOR_NEWCONTENT_COMEIN_AND_WAIT_FOR_CONSUMER]) {
			TimeTool::millisleep(500);
		}

		ostringstream gpioMonitorInfo;
		int result = GpioConfig(GetGpioDrv()).Get(param);
		if (result != EXIT_SUCCESS) {
			MessageCenter::Instance() << "GpioConfig Get failed: " << endl;
			return gpioMonitorInfo.str();
		}

		output.clear();
		vector<string> pinVals;
		for_each(get<1>(param).begin(), get<1>(param).end(), [&pinVals, &output](bool active) { 
			ostringstream os(std::ios_base::ate); 
			os << (active ? TerminalCtrl::FontColor(TerminalCtrl::Color::GREEN)+"High" : TerminalCtrl::FontColor(TerminalCtrl::Color::RED)+"Low"); 
			pinVals.emplace_back(os.str());
			output += (active ? "High," : "Low,");
		});

		ctrlBitFlag[GpioMonitorFunction::MONITOR_NEWCONTENT_COMEIN_AND_WAIT_FOR_CONSUMER] = true;
		if (flag != nullptr) {
			*flag = static_cast<int>(ctrlBitFlag.to_ulong());
		}

		return StringAlign::OutputTableRow(pinVals, tableColumnLen + 5);
	};

	MessageCenter::Instance().RefreshMessageUntill(gpioMonitor, [&flag]()->bool{ return (flag == nullptr) ? false : bitset<32>(*flag)[GpioMonitorFunction::EXIT_MONITOR_PROCESS]; });

	// 退出时设置死亡标志
	if (flag != nullptr) {
		*flag = 0x7fffffff;
	}

	MessageCenter::Instance() << TerminalCtrl::Reset() << "MonitorProcess exit" << endl;
}