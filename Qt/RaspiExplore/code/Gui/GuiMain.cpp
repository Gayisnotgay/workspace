#include <memory>
#include <vector>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtTest/QTest>
#include <QtGui/QIcon>
#include "AutoRegFactory/AutoRegFactory.h"
#include "GpioDrvWrapper/GpioDrvWrapper.h"
#include "ExploreRaspiAppGuiWidget.h"


using std::unique_ptr;
using std::vector;
using std::string;
using std::get;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ExploreRaspiAppGuiWidget widget;

    // 设置应用程序图标
    QIcon appIcon(QObject::tr(":/RaspberryIcon.ico"));
    app.setWindowIcon(appIcon);
    widget.setWindowIcon(appIcon);

    // 配置功能列表
    widget.SetFunList(get<1>(FuncFactory<int, const vector<string>&, string&, int*>::Instance().GetAllFuncInfo()));
    widget.SetGpioDrvList(get<1>(FuncFactory<unique_ptr<GpioDrvWrapper>, const string&>::Instance().GetAllFuncInfo()));

    widget.show();
    return app.exec();
}