#pragma once

#include "cglm/vec2.h"
#include "Userdata.h"
#include "sstream"

class Vec2 : public Userdata 
{
public:
	vec2 Vec2Val;
	Vec2(float x, float y);
	static int Register(LuauVM& vm);
	static int New(lua_State* L);
	int Index(lua_State* L) override;
	int NewIndex(lua_State* L) override;
	int Add(lua_State* L) override;
	int Sub(lua_State* L) override;
	int Mul(lua_State* L) override;
	int Div(lua_State* L) override;
	int ToString(lua_State* L) override;
};