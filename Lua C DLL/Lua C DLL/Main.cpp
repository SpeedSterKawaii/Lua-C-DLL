#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "Parser.h"
using namespace std;
#pragma warning(disable : 4996).
#define CPipeName "LuaCPipeName" /* This is the Pipe Name */

void CreateConsole(const char* Name) /* Bypasses and Creates Console */
{
	DWORD eee;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &eee);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	::SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(GetConsoleWindow(), SW_NORMAL);
}

void Init() /* This is the Main Thread Void */
{
	CreateConsole("Lua C Debug Console - SpeedSterKawaii");
	PrintConsole("Console Bypassed -> True");
	PrintConsole("Script Context -> Wait");
	Scanner::Scan(PAGE_READWRITE, (char*)&Roblox::ScriptContextADDR, (char*)"xxxx");
	PrintConsole("Script Context -> Done");
	CheckAddresses();
	PrintConsole("Lua C DLL -> Injected");
	std::cout << "Roblox ID -> " << Commands::GetRobloxUser << "\n";
	while (true)
	{
		std::cout << ">";
		std::string input;
		std::getline(std::cin, input);
		Commands::PrintLuaC(input, 1);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD Reason, void* Reservered)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		/* Create Thread meaning we Init the void "Init" */
		CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Init), 0, 0, 0);
	}
	return TRUE;
}

