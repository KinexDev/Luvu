#include "LuaTexture.h"

LuaTexture::LuaTexture(const char* path)
{
	tex = new Texture(path);
}

LuaTexture::~LuaTexture()
{
	delete tex;
}

int LuaTexture::Load(lua_State* L)
{
	if (!lua_isstring(L, 1))
		return 0;

	const char* str = lua_tostring(L, 1);

	PUSH_USERDATA(L, LuaTexture, str);
	return 1;
}

void LuaTexture::Register(lua_State* L)
{
	lua_newtable(L);
	PUSH_FUNCTION_AS_TABLE_KEY(L, &Load, "Load");
	lua_setglobal(L, "Texture");
}