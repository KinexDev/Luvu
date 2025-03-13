#pragma once
#include "lualib.h"
#include <luacode.h>
#include <iostream> 
#include <functional>
#include "Userdata.h"

class Userdata;

class LuauVM {
public:
	int DoString(const std::string &source);
	void PushGlobalFunction(const std::string &name, const lua_CFunction &function);
	void PushFunction(const lua_CFunction& function);
	template<typename T, typename... Args>
	void PushGlobalUserdata(const std::string& name, Args ...args);
	template<typename T, typename... Args>
	void PushUserdata(Args ...args);
	LuauVM();
	~LuauVM();
	lua_State* L;
	static std::unordered_map<lua_State*, LuauVM*> LtoVM;
	static LuauVM* FindVM(lua_State* L);
private:
	static void UserdataDestructor(void* userdata);
	static int IndexFunction(lua_State* L);
	static int NewIndexFunction(lua_State* L);
};

template<typename T, typename... Args>
inline void LuauVM::PushGlobalUserdata(const std::string& name, Args ...args)
{
	const char* nameCstr = name.c_str();

	lua_pushvalue(L, LUA_GLOBALSINDEX);
	PushUserdata<T>(args);
	lua_setglobal(L, nameCstr);
	lua_pop(L, 1);
}

template<typename T, typename... Args>
inline void LuauVM::PushUserdata(Args ...args)
{
	void* luaUserdata = lua_newuserdatadtor(L, sizeof(T), &UserdataDestructor);
	new (luaUserdata) T(std::forward<Args>(args)...);

	lua_newtable(L);
	PushFunction(&IndexFunction);
	lua_setfield(L, -2, "__index");
	PushFunction(&NewIndexFunction);
	lua_setfield(L, -2, "__newindex");

	lua_setmetatable(L, -2);
}
