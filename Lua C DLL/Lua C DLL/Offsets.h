#include <Windows.h>
#include <string>
#include "Retcheck.h"
extern "C" {
#include "Lua\lua.h"
#include "Lua\lapi.h"
#include "Lua\lualib.h"
#include "Lua\lstate.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lapi.h"
#include "Lua\lfunc.h"
#include "Lua\lobject.h"
}
#define Protect(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0)) /* ASLR / Protect */
#define r_lua_getglobal(r,e) r_lua_getfield(r, LUA_GLOBALSINDEX, (e))
#define r_lua_tostring(r,i)	r_lua_tolstring(r, (i), NULL)
using namespace std; /* Include Std for Reasons */

namespace States
{
	DWORD RoState; /* Dword */
	lua_State* LState; /* Lua State */
}

namespace Roblox /* This is the address namespace */
{
	static int Id1 = 20;
	static int Id2 = 112;

	static int Top = 20;
	static int Base = 24;

	typedef DWORD(__thiscall* gdm)();
	gdm getdatamodel = (gdm)(Protect(0x00FDC080)); /* No need to update this */

	typedef void(__fastcall* pushstring)(DWORD RL, const char* lol);
	pushstring r_lua_pushstring = (pushstring)(retcheckbypass(Protect(0x01527450)));

	typedef void(__fastcall* getfield)(DWORD RL, int w, const char* lol);
	getfield r_lua_getfield = (getfield)retcheckbypass(Protect(0x015268D0));

	typedef void(__cdecl* pcall)(DWORD RL, int on, int tw, int te);
	pcall r_lua_pcall = (pcall)retcheckbypass(Protect(0x01527030));

	typedef char* (__cdecl* tostring)(DWORD RL, int re, size_t* size);
	tostring r_lua_tolstring = (tostring)retcheckbypass((Protect(0x01528290)));

	typedef int(__cdecl* printr)(int, const char*, ...);
	printr r_lua_print = (printr)(Protect(0x00674960));

	DWORD ScriptContextADDR = Protect(0x0206CA34); /* Script Context (update) */
}

void PrintConsole(const char* msg)
{
	cout << "[LuaC]: " << msg << "\n";
}

void CheckAddresses() /* This is a bad way of checking addresses. */
{
	if (!Roblox::r_lua_getfield || !Roblox::r_lua_pcall || !Roblox::r_lua_print || !Roblox::r_lua_pushstring)
	{
		PrintConsole("Error -> All Addresses are Incorrect");
	}
	else
	{
		PrintConsole("Info -> All Addresses are Correct");
	}
}

namespace Scanner /* Script Context Scanner (do not touch) */
{
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; *mask; ++aob, ++mask, ++location) {
			__try {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	}
	DWORD find_Pattern(BYTE* pattern, char* mask, BYTE protection = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
		SYSTEM_INFO SI = { 0 };
		GetSystemInfo(&SI);
		DWORD start = (DWORD)SI.lpMinimumApplicationAddress;
		DWORD end = (DWORD)SI.lpMaximumApplicationAddress;
		MEMORY_BASIC_INFORMATION mbi;
		while (start < end && VirtualQuery((void*)start, &mbi, sizeof(mbi))) {
			if ((mbi.State & MEM_COMMIT) && (mbi.Protect & protection) && !(mbi.Protect & PAGE_GUARD)) {
				for (DWORD i = (DWORD)mbi.BaseAddress; i < (DWORD)mbi.BaseAddress + mbi.RegionSize; ++i) {
					if (compare((BYTE*)i, pattern, mask)) {
						return i;
					}
				}
			}
			start += mbi.RegionSize;
		}
		return 0;
	}
	int Scan(DWORD mode, char* content, char* mask) {
		return find_Pattern((BYTE*)content, mask, mode);
	}
}