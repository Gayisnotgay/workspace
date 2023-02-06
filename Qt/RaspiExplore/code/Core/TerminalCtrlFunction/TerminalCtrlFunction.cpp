#include "TerminalCtrlFunction.h"
#include <iostream>
#include <unistd.h>
#include "TerminalCtrl/TerminalCtrl.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

TERMINALCTRL_FUNC_AUTO_REG(TerminalCtrlFunction);

int TerminalCtrlFunction::Run(int argc, char* argv[])
{
	cout << "ClearScreen: " << endl;
	sleep(2);
	cout << TerminalCtrl::ClearScreen() << TerminalCtrl::Reset() << endl;
	
	cout << "==============Test FontColor Start==============" << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::BLACK) << "BLACK: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::RED) << "RED: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::GREEN) << "GREEN: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::YELLOW) << "YELLOW: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::BLUE) << "BLUE: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::PURPLE) << "PURPLE: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::DRAKGREEN) << "DRAKGREEN: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::WHITE) << "WHITE: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::FontColor(TerminalCtrl::Color::RED) << TerminalCtrl::Highlight() << "LIGHT_RED: " << TerminalCtrl::Reset() << endl;
	cout << "==============Test FontColor End==============" << endl;
	
	sleep(3);

	cout << "==============Test BackGroundColor Start==============" << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::BLACK) << "BLACK: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::RED) << "RED: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::GREEN) << "GREEN: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::YELLOW) << "YELLOW: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::BLUE) << "BLUE: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::PURPLE) << "PURPLE: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::DRAKGREEN) << "DRAKGREEN: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::WHITE) << "WHITE: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::BackGroundColor(TerminalCtrl::Color::RED) << TerminalCtrl::Highlight() << "LIGHT_RED: " << TerminalCtrl::Reset() << endl;
	cout << "==============Test BackGroundColor End==============" << endl;

	sleep(3);

	cout << TerminalCtrl::Shine() << "Shine: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::QuickShine() << "QuickShine: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::Strikeout() << "Strikeout: " << TerminalCtrl::Reset() << endl;
	cout << TerminalCtrl::ColorReverse() << "ColorReverse: " << TerminalCtrl::Reset() << endl;
	cout << "Hide: " << TerminalCtrl::Hide() << "can you see me ?" << TerminalCtrl::Reset() << endl;

	cout << "CursorUp(1): " << endl;
	sleep(3);
	cout << TerminalCtrl::CursorUp(1) << "After CursorUp(1), I am here" << TerminalCtrl::Reset() << endl;

	sleep(3);

	cout << "CursorDown(1): ";cout.flush();
	sleep(3);
	cout << TerminalCtrl::CursorDown(1) << "After CursorDown(1), I am here" << TerminalCtrl::Reset() << endl;

	sleep(3);

	cout << "CursorLeft(2): ";cout.flush();
	sleep(3);
	cout << TerminalCtrl::CursorLeft(2) << "After CursorLeft(2), I am here" << TerminalCtrl::Reset() << endl;

	sleep(3);


	cout << "CursorRight(2): ";cout.flush();
	sleep(3);
	cout << TerminalCtrl::CursorRight(2) << "After CursorRight(2), I am here" << TerminalCtrl::Reset() << endl;

	sleep(3);

	cout << "ClearFromCursorToLineEnd: ";
	cout << "CursorLeft(10): " << TerminalCtrl::CursorLeft(10);cout.flush();
	sleep(3);
	cout << TerminalCtrl::ClearFromCursorToLineEnd() << endl;

	sleep(3);

	cout << "CursorUp(1): " << endl;
	sleep(3);
	cout << TerminalCtrl::CursorUp(1);
	cout << "ReserveCursorPos: " << TerminalCtrl::ReserveCursorPos();
	cout << " i will say something random" << endl;
	cout << "RestoreCursorPos: " << endl;
	sleep(3);
	cout << TerminalCtrl::RestoreCursorPos();

	sleep(3);

	cout << "HideCursor: " << endl;
	sleep(3);
	cout << TerminalCtrl::HideCursor();

	sleep(3);

	cout << "ShowCursor: " << endl;
	sleep(3);
	cout << TerminalCtrl::ShowCursor();

	return EXIT_SUCCESS;
}