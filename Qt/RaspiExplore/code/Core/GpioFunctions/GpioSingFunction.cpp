#include "GpioSingFunction.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <thread>
#include "MessageCenter/MessageCenter.h"
#include "NumMscNotationParser/NumMscNotationParser.h"
#include "ParseConfig/ParseConfig.h"

using std::fstream;
using std::cout;
using std::endl;
using std::for_each;
using std::string;
using std::vector;
using std::thread;

int GpioSingFunction::Run(int argc, char* argv[])
{
	if (static_cast<size_t>(argc) != m_minParaLen + 1) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	vector<string> args;
	for (size_t index = 0; index < m_minParaLen; ++index) {
		args.emplace_back(argv[index]);
	}

	auto pinPara = ParseConfig::ParseAssignCfg(argv[m_minParaLen]);
	if (pinPara.empty() || (pinPara.size() != 1)) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}
	args.emplace_back(pinPara.begin()->first);
	args.emplace_back(pinPara.begin()->second);

	string output;
	return Run(args, output, nullptr);
}

int GpioSingFunction::Run(const vector<string>& args, string& output, int* flag)
{
    if (GpioFunction::Run(args, output, flag) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

	SingleGpioToneConfig toneCfg;
	if (ParseParam(vector<string>(args.begin() + m_minParaLen, args.end()), toneCfg) != EXIT_SUCCESS) {
		ShowFunctionUsage();
		return EXIT_FAILURE;
	}

	thread process([this](const SingleGpioToneConfig& cfg, int* ctrl){ GpioSing(cfg, ctrl); }, toneCfg, flag);
	if (flag == nullptr) {
		process.join();
	} else {
		process.detach();
	}

	return EXIT_SUCCESS;
}

void GpioSingFunction::GpioSing(const SingleGpioToneConfig& toneCfg, int* ctrlFlag)
{
	GpioToneConfig(GetGpioDrv()).ToneRing(toneCfg, GpioToneConfig::ToneRingType::ONCE, ctrlFlag);
	// 设置进程退出码
	if (ctrlFlag != nullptr) {
		*ctrlFlag = 0x7fffffff;
	}
	MessageCenter::Instance() << "GpioSing exit successfully" << endl;
}

int GpioSingFunction::ParseParam(const vector<string>& args, SingleGpioToneConfig& toneCfg)
{
	if (args.empty() || (args.size() != 2)) {
		return EXIT_FAILURE;
	}

	toneCfg.offset = stoul(args[0]);
	return ExtractSongData(args[1], toneCfg.tones);
}

void GpioSingFunction::ShowFunctionUsage()
{
	MessageCenter::Instance() << "Usage: " << m_program << " <chip> <line_offset0>=<song_filepath>" << endl;
}

int GpioSingFunction::ExtractSongData(const string& songFilename, vector<SingleGpioToneConfig::Pwm>& songData)
{
	vector<string> mscNotation;
	int result = ExtractNumberedMusicNotation(songFilename, mscNotation);
	if (result != EXIT_SUCCESS) {
		return result;
	}

	return ParseNumberedMusicNotation(mscNotation, songData);
}

int GpioSingFunction::ExtractNumberedMusicNotation(const std::string& songName, std::vector<std::string>& mscNotation)
{
	fstream songfile(songName, std::ios_base::in);
	if (!songfile) {
		MessageCenter::Instance() << "Cannot find the song file(" << songName << ")" << endl;
		return EXIT_FAILURE;
	}

	string data;
	while(songfile.good() && !songfile.eof()) {
		songfile >> data;
		if (songfile.good()) {
			mscNotation.emplace_back(data);
		}
	}

	return EXIT_SUCCESS;
}

int GpioSingFunction::ParseNumberedMusicNotation(const std::vector<std::string>& mscNotation, vector<SingleGpioToneConfig::Pwm>& songData)
{
	return NumMscNotationParser().Parse(mscNotation, songData);
}