#include "LuaColor.h"

LuaColor::LuaColor(float r, float g, float b, float a)
{
    vec4Val[0] = r;
    vec4Val[1] = g;
    vec4Val[2] = b;
    vec4Val[3] = a;
}

void LuaColor::Register(lua_State* L)
{
    lua_newtable(L);
    PUSH_FUNCTION_AS_TABLE_KEY(L, &New, "New");
    lua_setglobal(L, "Color");
}

int LuaColor::New(lua_State* L)
{
    float r = 0, g = 0, b = 0, a = 1.0f;

    if (lua_isnumber(L, 1)) r = (float)lua_tonumber(L, 1);
    if (lua_isnumber(L, 2)) g = (float)lua_tonumber(L, 2);
    if (lua_isnumber(L, 3)) b = (float)lua_tonumber(L, 3);
    if (lua_isnumber(L, 4)) a = (float)lua_tonumber(L, 4);

    PUSH_USERDATA(L, LuaColor, r, g, b, a);
    return 1;
}

int LuaColor::Index(lua_State* L)
{
    const char* name = lua_tostring(L, 2);

    if (strcmp(name, "r") == 0)
    {
        lua_pushnumber(L, vec4Val[0]);
        return 1;
    }
    if (strcmp(name, "g") == 0)
    {
        lua_pushnumber(L, vec4Val[1]);
        return 1;
    }
    if (strcmp(name, "b") == 0)
    {
        lua_pushnumber(L, vec4Val[2]);
        return 1;
    }
    if (strcmp(name, "a") == 0)
    {
        lua_pushnumber(L, vec4Val[3]);
        return 1;
    }

    return 0;
}

int LuaColor::NewIndex(lua_State* L)
{
    const char* name = lua_tostring(L, 2);

    if (!lua_isnumber(L, 3)) return 0;

    float value = (float)lua_tonumber(L, 3);

    if (strcmp(name, "r") == 0) vec4Val[0] = value;
    if (strcmp(name, "g") == 0) vec4Val[1] = value;
    if (strcmp(name, "b") == 0) vec4Val[2] = value;
    if (strcmp(name, "a") == 0) vec4Val[3] = value;

    return 0;
}

int LuaColor::ToString(lua_State* L)
{
    std::stringstream s;
    s << "(r: " << vec4Val[0] << ", g: " << vec4Val[1] << ", b: " << vec4Val[2] << ", a: " << vec4Val[3] << ")";
    lua_pushstring(L, s.str().c_str());
    return 1;
}
