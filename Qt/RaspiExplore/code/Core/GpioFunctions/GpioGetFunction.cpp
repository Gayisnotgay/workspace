#include "GpioGetFunction.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <tuple>
#include <functional>
#include "ParseConfig/ParseConfig.h"
#include "GpioDrvWrapper/GpioDrvWrapperGpiod.h"
#include "MessageCenter/MessageCenter.h"

using namespace gpiod;

using std::string;
using std::stoul;
using std::cout;
using std::endl;
using std::ostringstream;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::get;
using std::for_each;
using std::function;

int GpioGetFunction::Run(int argc, char* argv[])
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
		MessageCenter::Instance() << "GpioGetFunction failed: " << endl;
        return EXIT_FAILURE;
    }

	MessageCenter::Instance() << output << endl;

	return EXIT_SUCCESS;
}

int GpioGetFunction::Run(const vector<string>& args, string& output, int* flag)
{
    if (GpioFunction::Run(args, output, flag) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

	GpioAccessParam param;
	if (ParseParam(vector<string>(args.begin() + m_minParaLen, args.end()), param) != EXIT_SUCCESS) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	int result = GpioConfig(GetGpioDrv()).Get(param);
    if (result != EXIT_SUCCESS) {
		MessageCenter::Instance() << "GpioConfig Get failed: " << endl;
        return EXIT_FAILURE;
    }

	ostringstream os(std::ios_base::ate);
	for_each(get<1>(param).begin(), get<1>(param).end(), [&os](bool active) { os << (active ? "High," : "Low,"); });
	output = os.str();

	return EXIT_SUCCESS;
}

int GpioGetFunction::ParseParam(const vector<string>& args, GpioAccessParam& param)
{
	if (args.empty()) {
		return EXIT_FAILURE;
	}

	for_each(args.begin(), args.end(), [&param](const string& arg) { get<0>(param).push_back(stoul(arg)); });

	return EXIT_SUCCESS;
}

void GpioGetFunction::ShowFunctionUsage()
{
	MessageCenter::Instance() << "Usage: " << m_program << " <chip> <line_offset0>,..." << endl;
}
