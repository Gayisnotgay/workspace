#include "GpioPeriodSetFunction.h"
#include <algorithm>
#include "MessageCenter/MessageCenter.h"
#include "ParseConfig/ParseConfig.h"

using std::for_each;
using std::string;
using std::vector;
using std::endl;

int GpioPeriodSetFunction::Run(int argc, char* argv[])
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

int GpioPeriodSetFunction::Run(const vector<string>& args, string& output, int* flag)
{
    if (GpioFunction::Run(args, output, flag) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

	vector<SingleGpioPeriodConfig> param;
	if (ParseParam(vector<string>(args.begin() + m_minParaLen, args.end()), param) != EXIT_SUCCESS) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	GpioPeriodConfig cfg(GetGpioDrv());
    cfg.ConfigCandence(param);
    cfg.Run(flag);

	return EXIT_SUCCESS;
}

int GpioPeriodSetFunction::ParseParam(const vector<string>& args, vector<SingleGpioPeriodConfig>& param)
{
	if (args.empty() || (args.size() % 4 != 0)) {
		return EXIT_FAILURE;
	}

	for (size_t idx = 0; idx < args.size(); idx += 4) {
        SingleGpioPeriodConfig cfg;
        cfg.offset = stoul(args[idx]);
        // 解析initValue和switchTimeInMilliSecond
        cfg.initValue = (stoul(args[idx + 1]) != 0);
        for_each(args.begin() + idx + 2, args.begin() + idx + 4, [&cfg](const string& interval){ cfg.switchTimeInMilliSecond.emplace_back(stoul(interval));});
        param.emplace_back(cfg);
	}

	return EXIT_SUCCESS;
}

vector<string> GpioPeriodSetFunction::SplitParam(int argc, char* argv[])
{
	vector<string> splitParam;
	for (int i = 0; i < argc; i++) {
        auto oneCfg = ParseConfig::ParsePattern(argv[i], "\\s*(\\w+)\\s*[=,]?");
		if (oneCfg.empty()) {
			MessageCenter::Instance() << "GpioSetFunction::SplitParam, invalid offset=value mapping: " << argv[i] << endl;
            break;
        }
        splitParam.insert(splitParam.end(), oneCfg.begin(), oneCfg.end());
	}

	return splitParam;
}

void GpioPeriodSetFunction::ShowFunctionUsage()
{
	MessageCenter::Instance() << "Usage: " << m_program << " <chip> <line_offset0>=<initValue0>,Cadence:100,200,100... <line_offsetn>=..." << endl;
}
