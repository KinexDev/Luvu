#include "Vec2.h"


Vec2::Vec2(float x, float y)
{
	Vec2Val[0] = x;
	Vec2Val[1] = y;
}

int Vec2::Register(LuauVM& vm)
{
	lua_newtable(vm.L);
	vm.PushFunction(New);
	lua_setfield(vm.L, -2, "New");
	lua_setglobal(vm.L, "Vec2");
	return 0;
}

int Vec2::New(lua_State* L)
{
	LuauVM* vm = LuauVM::FindVM(L);
	float x = 0;
	float y = 0;

	if (lua_isnumber(L, 1))
		x = (float)lua_tonumber(L, 1);

	if (lua_isnumber(L, 2))
		y = (float)lua_tonumber(L, 2);

	vm->PushUserdata<Vec2>(x, y);
	return 1;
}

int Vec2::Index(lua_State* L)
{
	LuauVM* vm = LuauVM::FindVM(L);
	std::string name = std::string(lua_tostring(L, 2));

	if (name == "x")
	{
		lua_pushnumber(L, Vec2Val[0]);
		return 1;
	}

	if (name == "y")
	{
		lua_pushnumber(L, Vec2Val[1]);
		return 1;
	}

	return 0;
}

int Vec2::NewIndex(lua_State* L)
{
	std::string name = std::string(lua_tostring(L, 2));

	if (name == "x")
	{
		if (!lua_isnumber(L, 3))
		{
			return 0;
		}

		Vec2Val[0] = (float)lua_tonumber(L, 3);
	}

	if (name == "y")
	{
		if (!lua_isnumber(L, 3))
		{
			return 0;
		}

		Vec2Val[1] = (float)lua_tonumber(L, 3);
	}

	return 0;
}

int Vec2::Add(lua_State* L)
{
	if (!lua_isuserdata(L, 2))
		return 0;

	LuauVM* vm = LuauVM::FindVM(L);

	Vec2* otherVec2 = static_cast<Vec2*>(lua_touserdata(L, 2));

	if (otherVec2)
	{
		vec2 result;
		glm_vec2_add(Vec2Val, otherVec2->Vec2Val, result);
		vm->PushUserdata<Vec2>(result[0], result[1]);
		return 1;
	}

	return 0;
}

int Vec2::Sub(lua_State* L)
{
	if (!lua_isuserdata(L, 2))
		return 0;

	LuauVM* vm = LuauVM::FindVM(L);

	Vec2* otherVec2 = static_cast<Vec2*>(lua_touserdata(L, 2));

	if (otherVec2)
	{
		vec2 result;
		glm_vec2_sub(Vec2Val, otherVec2->Vec2Val, result);
		vm->PushUserdata<Vec2>(result[0], result[1]);
		return 1;
	}

	return 0;
}

int Vec2::Mul(lua_State* L)
{
	if (!lua_isnumber(L, 2))
		return 0;

	LuauVM* vm = LuauVM::FindVM(L);

	float scale = (float)lua_tonumber(L, 2);

	vec2 result;
	glm_vec2_scale(Vec2Val, scale, result);
	vm->PushUserdata<Vec2>(result[0], result[1]);
	return 1;
}

int Vec2::Div(lua_State* L)
{
	if (!lua_isnumber(L, 2))
		return 0;

	LuauVM* vm = LuauVM::FindVM(L);

	float scale = (float)lua_tonumber(L, 2);
	vec2 result;
	glm_vec2_divs(Vec2Val, scale, result);
	vm->PushUserdata<Vec2>(result[0], result[1]);
	return 1;
}

int Vec2::ToString(lua_State* L)
{
	std::stringstream s;
	s << "(x: " << Vec2Val[0] << ", y: " << Vec2Val[1] << ")";
	lua_pushstring(L, s.str().c_str());
	return 1;
}