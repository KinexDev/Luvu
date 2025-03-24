#pragma once
#include "Game.h"
namespace Graphics {
	void Register(lua_State* L);
	int DrawImage(lua_State* L);
	int GetWidth(lua_State* L);
	int GetHeight(lua_State* L);
	int DrawRectangle(lua_State* L);
	int DrawLine(lua_State* L);
	int SetWindowTitle(lua_State* L);
	int SetWindowSize(lua_State* L);
	int SetWindowPos(lua_State* L);
	int SetWindowResizable(lua_State* L);
	int SetFullscreen(lua_State* L);
	int SetVSync(lua_State* L);
}