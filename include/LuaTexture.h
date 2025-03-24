#pragma once

#include "Game.h"
#include "Texture.h"

class LuaTexture : public Userdata 
{
public:
	LuaTexture(const char* path);
	~LuaTexture();
	static void Register(lua_State* L);
	static int Load(lua_State* L);
	Texture* tex;
};