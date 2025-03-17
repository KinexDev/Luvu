#pragma once

#include "cglm/vec2.h"
#include "../LuauVM/Userdata.h"
#include "sstream"

class LuaVec2 : public Userdata 
{
public:
	vec2 vec2Val;
	LuaVec2(float x, float y);
	static void Register(lua_State* L);
	static int New(lua_State* L);
	static int Dist(lua_State* L);
	static int Dot(lua_State* L);
	static int Reflect(lua_State* L);
	static int Lerp(lua_State* L);
	static int DistSquared(lua_State* L);
	static int Normalize(lua_State* L);
	static int Length(lua_State* L);
	int Index(lua_State* L) override;
	int NewIndex(lua_State* L) override;
	int Add(lua_State* L) override;
	int Sub(lua_State* L) override;
	int Mul(lua_State* L) override;
	int Div(lua_State* L) override;
	int Eq(lua_State* L) override;
	int ToString(lua_State* L) override;
};