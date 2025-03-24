#include "LuaInput.h"

void LuaInput::Register(lua_State* L)
{
	lua_newtable(L);
	PUSH_FUNCTION_AS_TABLE_KEY(L, &GetKey, "GetKey");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &GetMouseButton, "GetMouseButton");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &GetMousePosition, "GetMousePosition");

	lua_setglobal(L, "Input");

	lua_newtable(L);
	lua_pushinteger(L, GLFW_KEY_A);
	lua_setfield(L, -2, "A");
	lua_pushinteger(L, GLFW_KEY_B);
	lua_setfield(L, -2, "B");
	lua_pushinteger(L, GLFW_KEY_C);
	lua_setfield(L, -2, "C");
	lua_pushinteger(L, GLFW_KEY_D);
	lua_setfield(L, -2, "D");
	lua_pushinteger(L, GLFW_KEY_E);
	lua_setfield(L, -2, "E");
	lua_pushinteger(L, GLFW_KEY_F);
	lua_setfield(L, -2, "F");
	lua_pushinteger(L, GLFW_KEY_G);
	lua_setfield(L, -2, "G");
	lua_pushinteger(L, GLFW_KEY_H);
	lua_setfield(L, -2, "H");
	lua_pushinteger(L, GLFW_KEY_I);
	lua_setfield(L, -2, "I");
	lua_pushinteger(L, GLFW_KEY_J);
	lua_setfield(L, -2, "J");
	lua_pushinteger(L, GLFW_KEY_K);
	lua_setfield(L, -2, "K");
	lua_pushinteger(L, GLFW_KEY_L);
	lua_setfield(L, -2, "L");
	lua_pushinteger(L, GLFW_KEY_M);
	lua_setfield(L, -2, "M");
	lua_pushinteger(L, GLFW_KEY_N);
	lua_setfield(L, -2, "N");
	lua_pushinteger(L, GLFW_KEY_O);
	lua_setfield(L, -2, "O");
	lua_pushinteger(L, GLFW_KEY_P);
	lua_setfield(L, -2, "P");
	lua_pushinteger(L, GLFW_KEY_Q);
	lua_setfield(L, -2, "Q");
	lua_pushinteger(L, GLFW_KEY_R);
	lua_setfield(L, -2, "R");
	lua_pushinteger(L, GLFW_KEY_S);
	lua_setfield(L, -2, "S");
	lua_pushinteger(L, GLFW_KEY_T);
	lua_setfield(L, -2, "T");
	lua_pushinteger(L, GLFW_KEY_U);
	lua_setfield(L, -2, "U");
	lua_pushinteger(L, GLFW_KEY_V);
	lua_setfield(L, -2, "V");
	lua_pushinteger(L, GLFW_KEY_W);
	lua_setfield(L, -2, "W");
	lua_pushinteger(L, GLFW_KEY_X);
	lua_setfield(L, -2, "X");
	lua_pushinteger(L, GLFW_KEY_Y);
	lua_setfield(L, -2, "Y");
	lua_pushinteger(L, GLFW_KEY_Z);
	lua_setfield(L, -2, "Z");

	lua_pushinteger(L, GLFW_KEY_0);
	lua_setfield(L, -2, "Key0");
	lua_pushinteger(L, GLFW_KEY_1);
	lua_setfield(L, -2, "Key1");
	lua_pushinteger(L, GLFW_KEY_2);
	lua_setfield(L, -2, "Key2");
	lua_pushinteger(L, GLFW_KEY_3);
	lua_setfield(L, -2, "Key3");
	lua_pushinteger(L, GLFW_KEY_4);
	lua_setfield(L, -2, "Key4");
	lua_pushinteger(L, GLFW_KEY_5);
	lua_setfield(L, -2, "Key5");
	lua_pushinteger(L, GLFW_KEY_6);
	lua_setfield(L, -2, "Key6");
	lua_pushinteger(L, GLFW_KEY_7);
	lua_setfield(L, -2, "Key7");
	lua_pushinteger(L, GLFW_KEY_8);
	lua_setfield(L, -2, "Key8");
	lua_pushinteger(L, GLFW_KEY_9);
	lua_setfield(L, -2, "Key9");

	lua_pushinteger(L, GLFW_KEY_SPACE);
	lua_setfield(L, -2, "Space");
	lua_pushinteger(L, GLFW_KEY_ENTER);
	lua_setfield(L, -2, "Enter");
	lua_pushinteger(L, GLFW_KEY_TAB);
	lua_setfield(L, -2, "Tab");
	lua_pushinteger(L, GLFW_KEY_BACKSPACE);
	lua_setfield(L, -2, "Backspace");
	lua_pushinteger(L, GLFW_KEY_ESCAPE);
	lua_setfield(L, -2, "Escape");
	lua_pushinteger(L, GLFW_KEY_LEFT_SHIFT);
	lua_setfield(L, -2, "LeftShift");
	lua_pushinteger(L, GLFW_KEY_RIGHT_SHIFT);
	lua_setfield(L, -2, "RightShift");
	lua_pushinteger(L, GLFW_KEY_LEFT_CONTROL);
	lua_setfield(L, -2, "LeftControl");
	lua_pushinteger(L, GLFW_KEY_RIGHT_CONTROL);
	lua_setfield(L, -2, "RightControl");
	lua_pushinteger(L, GLFW_KEY_LEFT_ALT);
	lua_setfield(L, -2, "LeftAlt");
	lua_pushinteger(L, GLFW_KEY_RIGHT_ALT);
	lua_setfield(L, -2, "RightAlt");
	lua_pushinteger(L, GLFW_KEY_UP);
	lua_setfield(L, -2, "Up");
	lua_pushinteger(L, GLFW_KEY_DOWN);
	lua_setfield(L, -2, "Down");
	lua_pushinteger(L, GLFW_KEY_LEFT);
	lua_setfield(L, -2, "Left");
	lua_pushinteger(L, GLFW_KEY_RIGHT);
	lua_setfield(L, -2, "Right");

	lua_setglobal(L, "Key");
}

int LuaInput::GetKey(lua_State* L)
{
	if (!lua_isnumber(L, 1))
		return 0;
	int key = (int)lua_tonumber(L, 1);
	Game& game = Game::Instance();
	if (glfwGetKey(game.window, key) == GLFW_PRESS)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}

int LuaInput::GetMouseButton(lua_State* L)
{
	if (!lua_isnumber(L, 1))
		return 0;
	int button = (int)lua_tonumber(L, 1);
	Game& game = Game::Instance();
	if (glfwGetMouseButton(game.window, button) == GLFW_PRESS)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}

int LuaInput::GetMousePosition(lua_State* L)
{
	double xpos, ypos;
	Game& game = Game::Instance();
	glfwGetCursorPos(game.window, &xpos, &ypos);
	PUSH_USERDATA(L, LuaVec2, xpos, ypos);
	return 1;
}
