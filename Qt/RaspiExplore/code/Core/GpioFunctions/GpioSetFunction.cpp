#include "GpioSetFunction.h"
#include <cstdlib>
#include <iostream>
#include <tuple>
#include "ParseConfig/ParseConfig.h"
#include "MessageCenter/MessageCenter.h"

using std::string;
using std::stoul;
using std::cout;
using std::endl;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::get;
using std::for_each;

int GpioSetFunction::Run(int argc, char* argv[])
{
	if (static_cast<size_t>(argc) < m_minParaLen + 1) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	vector<string> pins = SplitParam(argc - m_minParaLen, argv + m_minParaLen);
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

	return Run(args, output, nullptr);
}

int GpioSetFunction::Run(const std::vector<std::string>& args, std::string& output, int* flag)
{
    if (GpioFunction::Run(args, output, flag) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

	GpioAccessParam param;
	if (ParseParam(vector<string>(args.begin() + m_minParaLen, args.end()), param) != EXIT_SUCCESS) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	GpioConfig(GetGpioDrv()).Set(param);

	return EXIT_SUCCESS;
}

vector<string> GpioSetFunction::SplitParam(int argc, char* argv[])
{
	vector<string> splitParam;
	for (int i = 0; i < argc; i++) {
        auto oneCfg = ParseConfig::ParseAssignCfg(argv[i]);
		if (oneCfg.empty()) {
			MessageCenter::Instance() << "GpioSetFunction::SplitParam, invalid offset=value mapping: " << argv[i] << endl;
            break;
        }

		splitParam.emplace_back(oneCfg.begin()->first);
		splitParam.emplace_back(oneCfg.begin()->second);
	}

	return splitParam;
}

int GpioSetFunction::ParseParam(const vector<string>& args, GpioAccessParam& param)
{
	if (args.empty() || (args.size() % 2 != 0)) {
		return EXIT_FAILURE;
	}

	for (size_t idx = 0; idx < args.size(); idx += 2) {
		get<0>(param).push_back(stoul(args[idx]));
		get<1>(param).push_back((stoul(args[idx + 1]) == 1));
	}

	return EXIT_SUCCESS;
}

void GpioSetFunction::ShowFunctionUsage()
{
	MessageCenter::Instance() << "Usage: " << m_program << " <chip> <line_offset0>=<value0> ..." << endl;
}
