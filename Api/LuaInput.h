#pragma once
#include "../LuauVM/Userdata.h"
#include "../Game.h"

namespace LuaInput {
	void Register(lua_State* L);
	int GetKey(lua_State* L);
	int GetKeyDown(lua_State* L);
	int GetKeyUp(lua_State* L);
}