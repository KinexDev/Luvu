#include "LuaVec2.h"


LuaVec2::LuaVec2(float x, float y)
{
	vec2Val[0] = x;
	vec2Val[1] = y;
}

void LuaVec2::Register(lua_State* L)
{
	lua_newtable(L);
	PUSH_FUNCTION_AS_TABLE_KEY(L, &New, "New");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &Dist, "Dist");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &DistSquared, "DistSquared");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &Lerp, "Lerp");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &Dot, "Dot");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &Reflect, "Reflect");
	lua_setglobal(L, "Vec2");
}

int LuaVec2::New(lua_State* L)
{
	float x = 0;
	float y = 0;

	if (lua_isnumber(L, 1))
		x = (float)lua_tonumber(L, 1);

	if (lua_isnumber(L, 2))
		y = (float)lua_tonumber(L, 2);

	PUSH_USERDATA(L, LuaVec2, x, y);
	return 1;
}

int LuaVec2::Dist(lua_State* L)
{
	if (!lua_isuserdata(L, 1) && !lua_isuserdata(L, 2))
		return 0;

	LuaVec2* currentVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 2));

	if (currentVec2 && otherVec2)
	{
		float dist = sqrt(glm_vec2_distance2(currentVec2->vec2Val, otherVec2->vec2Val));
		lua_pushnumber(L, dist);
		return 1;
	}
	return 0;
}

int LuaVec2::Dot(lua_State* L)
{
	if (!lua_isuserdata(L, 1) && !lua_isuserdata(L, 2))
		return 0;

	LuaVec2* currentVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 2));

	if (currentVec2 && otherVec2)
	{
		float dist = glm_vec2_dot(currentVec2->vec2Val, otherVec2->vec2Val);
		lua_pushnumber(L, dist);
		return 1;
	}
	return 0;
}

int LuaVec2::Reflect(lua_State* L)
{
	if (!lua_isuserdata(L, 1) && !lua_isuserdata(L, 2))
		return 0;

	LuaVec2* currentVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 2));

	if (currentVec2 && otherVec2)
	{
		vec2 result;
		glm_vec2_reflect(currentVec2->vec2Val, otherVec2->vec2Val, result);
		PUSH_USERDATA(L, LuaVec2, result[0], result[1]);
		return 1;
	}
	return 0;
}

int LuaVec2::Lerp(lua_State* L)
{
	if (!lua_isuserdata(L, 1) && !lua_isuserdata(L, 2) && !lua_isnumber(L, 3))
		return 0;

	LuaVec2* currentVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 2));
	float num = lua_tonumber(L, 3);

	if (currentVec2 && otherVec2)
	{
		vec2 result;
		glm_vec2_lerp(currentVec2->vec2Val, otherVec2->vec2Val, num, result);
		PUSH_USERDATA(L, LuaVec2, result[0], result[1]);
		return 1;
	}
	return 0;
}

int LuaVec2::DistSquared(lua_State* L)
{
	if (!lua_isuserdata(L, 1) && !lua_isuserdata(L, 2))
		return 0;

	LuaVec2* currentVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 2));

	if (currentVec2 && otherVec2)
	{
		float dist = glm_vec2_distance2(currentVec2->vec2Val, otherVec2->vec2Val);
		lua_pushnumber(L, dist);
		return 1;
	}
	return 0;
}

int LuaVec2::Normalize(lua_State* L)
{
	if (!lua_isuserdata(L, 1))
		return 0;

	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	if (otherVec2)
	{
		vec2 vec2 = { otherVec2->vec2Val[0], otherVec2->vec2Val[1] };
		glm_vec2_normalize(vec2);
		PUSH_USERDATA(L, LuaVec2, vec2[0], vec2[1]);
		return 1;
	}
	return 0;
}

int LuaVec2::Length(lua_State* L)
{
	if (!lua_isuserdata(L, 1))
		return 0;

	LuaVec2* currentVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));

	if (currentVec2)
	{
		float length = sqrt(currentVec2->vec2Val[0] * currentVec2->vec2Val[0] +
			currentVec2->vec2Val[1] * currentVec2->vec2Val[1]);
		lua_pushnumber(L, length);
		return 1;
	}
	return 0;
}

int LuaVec2::Index(lua_State* L)
{
	const char* name = lua_tostring(L, 2);

	if (strcmp(name, "x") == 0)
	{
		lua_pushnumber(L, vec2Val[0]);
		return 1;
	}

	if (strcmp(name, "y") == 0)
	{
		lua_pushnumber(L, vec2Val[1]);
		return 1;
	}

	if (strcmp(name, "Normalize") == 0)
	{
		PUSH_FUNCTION(L, &Normalize);
		return 1;
	}

	if (strcmp(name, "Length") == 0)
	{
		PUSH_FUNCTION(L, &Length);
		return 1;
	}

	return 0;
}

int LuaVec2::NewIndex(lua_State* L)
{
	const char* name = lua_tostring(L, 2);

	if (strcmp(name, "x") == 0)
	{
		if (!lua_isnumber(L, 3))
		{
			return 0;
		}

		vec2Val[0] = (float)lua_tonumber(L, 3);
	}

	if (strcmp(name, "y") == 0)
	{
		if (!lua_isnumber(L, 3))
		{
			return 0;
		}

		vec2Val[1] = (float)lua_tonumber(L, 3);
	}

	return 0;
}

int LuaVec2::Add(lua_State* L)
{
	if (!lua_isuserdata(L, 2))
		return 0;

	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 2));

	if (otherVec2)
	{
		vec2 result;
		glm_vec2_add(vec2Val, otherVec2->vec2Val, result);
		PUSH_USERDATA(L, LuaVec2, result[0], result[1]);
		return 1;
	}

	return 0;
}

int LuaVec2::Sub(lua_State* L)
{
	if (!lua_isuserdata(L, 2))
		return 0;

	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 2));

	if (otherVec2)
	{
		vec2 result;
		glm_vec2_sub(vec2Val, otherVec2->vec2Val, result);
		PUSH_USERDATA(L, LuaVec2, result[0], result[1]);
		return 1;
	}

	return 0;
}

int LuaVec2::Mul(lua_State* L)
{
	if (!lua_isnumber(L, 2))
		return 0;

	float scale = (float)lua_tonumber(L, 2);

	vec2 result;
	glm_vec2_scale(vec2Val, scale, result);

	PUSH_USERDATA(L, LuaVec2, result[0], result[1]);
	return 1;
}

int LuaVec2::Div(lua_State* L)
{
	if (!lua_isnumber(L, 2))
		return 0;

	float scale = (float)lua_tonumber(L, 2);
	vec2 result;
	glm_vec2_divs(vec2Val, scale, result);
	PUSH_USERDATA(L, LuaVec2, result[0], result[1]);
	return 1;
}

int LuaVec2::Eq(lua_State* L)
{
	if (!lua_isuserdata(L, 2))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	LuaVec2* otherVec2 = static_cast<LuaVec2*>(lua_touserdata(L, 2));

	if (otherVec2)
	{
		lua_pushboolean(L, (vec2Val[0] == otherVec2->vec2Val[0]) && (vec2Val[1] == otherVec2->vec2Val[1]));
		return 1;
	}

	lua_pushboolean(L, 0);
	return 1;
}

int LuaVec2::ToString(lua_State* L)
{
	std::stringstream s;
	s << "(x: " << vec2Val[0] << ", y: " << vec2Val[1] << ")";
	lua_pushstring(L, s.str().c_str());
	return 1;
}