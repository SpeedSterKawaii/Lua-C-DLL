#include "Offsets.h"
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

const char* GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int FindFirstClass(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 44);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}

namespace Commands /* This is Lua C Commands */
{
	void WarnLuaC(const char* warn)
	{
		Roblox::r_lua_getfield(States::RoState, LUA_GLOBALSINDEX, "warn");
		Roblox::r_lua_pushstring(States::RoState, warn);
		Roblox::r_lua_pcall(States::RoState, 1, 0, 0);
	}

	void PrintLuaC(std::string print, int lol)
	{
		Roblox::r_lua_print(lol, print.c_str());
	}

	std::string GetRobloxUser() {
		Roblox::r_lua_getfield(States::RoState, LUA_GLOBALSINDEX, "game");
		Roblox::r_lua_getfield(States::RoState, -1, "Players");
		Roblox::r_lua_getfield(States::RoState, -1, "LocalPlayer");
		Roblox::r_lua_getfield(States::RoState, -1, "Name");
		return Roblox::r_lua_tolstring(States::RoState, -1, NULL);
	}

	std::string GetRobloxHealth() {
		Roblox::r_lua_getfield(States::RoState, LUA_GLOBALSINDEX, "game");
		Roblox::r_lua_getfield(States::RoState, -1, "Players");
		Roblox::r_lua_getfield(States::RoState, -1, "LocalPlayer");
		Roblox::r_lua_getfield(States::RoState, -1, "Character");
		Roblox::r_lua_getfield(States::RoState, -1, "Humanoid");
		Roblox::r_lua_getfield(States::RoState, -1, "Health");
		return Roblox::r_lua_tolstring(States::RoState, -1, NULL);
	}
}