#pragma once
#include "Userdata.h"
#include "Game.h"

namespace LuaInput {
	void Register(lua_State* L);
	int GetKey(lua_State* L);
	int GetMouseButton(lua_State* L);
	int GetMousePosition(lua_State* L);
}