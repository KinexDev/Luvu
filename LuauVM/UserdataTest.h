#pragma once

#include "Userdata.h"

class UserdataTest : public Userdata
{
public:
	int num;
	UserdataTest(int number);
	static int Call(lua_State* L);
	static int New(lua_State* L);
	static int Register(LuauVM &vm);
	int Index(lua_State* L) override;
	int NewIndex(lua_State* L) override;
};