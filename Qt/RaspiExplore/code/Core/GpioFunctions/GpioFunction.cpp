#include "GpioFunction.h"
#include <algorithm>
#include "MessageCenter/MessageCenter.h"

using std::string;
using std::vector;
using std::endl;
using std::for_each;
using std::unique_ptr;

int GpioFunction::Run(int argc, char* argv[])
{
    if ((argc > 0) && (argv == nullptr)) {
        return EXIT_FAILURE;
    }

    vector<string> args(argc, "");
    for (int index = 0; index < argc; ++index) {
        args[index] = argv[index];
    }

    string output;
    return Run(args, output);
}

int GpioFunction::Run(const vector<string>& args, string& output, int* flag/* = nullptr*/)
{
    if (args.size() < m_minParaLen) {
        return EXIT_FAILURE;
    }

    m_program.assign(args[0]);

    return CreateGpioDrv(args[1]);
}

int GpioFunction::CreateGpioDrv(const string& drvType)
{
    if (m_gpioDrv == nullptr) {
        m_gpioDrv = FuncFactory<unique_ptr<GpioDrvWrapper>, const string&>::Instance().RunFunc(drvType, "gpiochip0");
    }

    return m_gpioDrv ? EXIT_SUCCESS : EXIT_FAILURE;
}

GpioDrvWrapper& GpioFunction::GetGpioDrv()
{
    return *m_gpioDrv;
}

void GpioFunction::ShowFunctionUsage()
{
    MessageCenter::Instance() << "Usage: " << m_program << " <GpioDrv> <GpioPin>..." << endl;
}