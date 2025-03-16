#pragma once
#include "../LuauVM/Userdata.h"
#include "cglm/vec4.h"
#include "sstream"

class LuaColor : public Userdata {
public:
	vec4 vec4Val;
	LuaColor(float r, float g, float b, float a);
	static void Register(lua_State* L);
	static int New(lua_State* L);
	int Index(lua_State* L) override;
	int NewIndex(lua_State* L) override;
	int ToString(lua_State* L) override;
};