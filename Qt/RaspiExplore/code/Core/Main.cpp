#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>
#include <iomanip>
#include <typeinfo>
#include "AutoRegFactory/AutoRegFactory.h"
#include "MessageCenter/MessageCenter.h"

using std::cin;
using std::cout;
using std::endl;
using std::function;
using std::vector;
using std::for_each;
using std::get;

void ShowProgramHelp(int argc, char* argv[])
{
	cout << "__cplusplus: " << __cplusplus << endl;

	MessageCenter::Instance() << "Usage: " << argv[0] << " <function> <functionParas>" << endl
		<< "function lists are: " << endl
		<< get<1>(FuncFactory<int, int, char**>::Instance().GetAllFuncInfo()) << endl;
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		ShowProgramHelp(argc, argv);
		return EXIT_FAILURE;
	}

	int result = FuncFactory<int, int, char**>::Instance().RunFunc(argv[1], argc - 1, argv + 1);
	if (result != EXIT_SUCCESS) {
		ShowProgramHelp(argc, argv);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS; 
}