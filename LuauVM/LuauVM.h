#pragma once
#include "lualib.h"
#include <luacode.h>
#include <iostream> 
#include <functional>
#include "Userdata.h"

class Userdata;

class LuauVM {
public:
	int DoString(const std::string &source, int results = 0);
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
	static int AddFunction(lua_State* L);
	static int SubFunction(lua_State* L);
	static int MulFunction(lua_State* L);
	static int DivFunction(lua_State* L);
	static int ToStringFunction(lua_State* L);
	static int ConcatFunction(lua_State* L);
	static int EqFunction(lua_State* L);
	static int LtFunction(lua_State* L);
	static int LeFunction(lua_State* L);
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

	// this is awful...
	PushFunction(&IndexFunction);
	lua_setfield(L, -2, "__index");
	PushFunction(&NewIndexFunction);
	lua_setfield(L, -2, "__newindex");
	PushFunction(&AddFunction);
	lua_setfield(L, -2, "__add");
	PushFunction(&SubFunction);
	lua_setfield(L, -2, "__sub");
	PushFunction(&MulFunction);
	lua_setfield(L, -2, "__mul");
	PushFunction(&DivFunction);
	lua_setfield(L, -2, "__div");
	PushFunction(&ToStringFunction);
	lua_setfield(L, -2, "__tostring");
	PushFunction(&ConcatFunction);
	lua_setfield(L, -2, "__concat");
	PushFunction(&EqFunction);
	lua_setfield(L, -2, "__eq");
	PushFunction(&LtFunction);
	lua_setfield(L, -2, "__lt");
	PushFunction(&LeFunction);
	lua_setfield(L, -2, "__le");

	lua_setmetatable(L, -2);
}
