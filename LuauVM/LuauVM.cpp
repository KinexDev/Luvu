#include "LuauVM.h"

std::unordered_map<lua_State*, LuauVM*> LuauVM::LtoVM;

int LuauVM::DoString(const std::string &source, int results)
{
	size_t bytecodeSize = 0;

	const char* sourceCstr = source.c_str();
	char* bytecode = luau_compile(sourceCstr, strlen(sourceCstr), nullptr, &bytecodeSize);

	if (!luau_load(L, "Script", bytecode, bytecodeSize, 0))
	{
		if (lua_pcall(L, 0, results, 0))
		{
			const char* error = lua_tostring(L, 1);
			std::cout << error;
			delete[] bytecode;
			return 1;
		}
	}
	else
	{
		std::cerr << "Error loading the script!";
		delete[] bytecode;
		return 1;
	}

	delete[] bytecode;
	return 0;
}

void LuauVM::PushGlobalFunction(const std::string &name, const lua_CFunction& function)
{
	const char* nameCstr = name.c_str();

	lua_pushvalue(L, LUA_GLOBALSINDEX);
	PushFunction(function);
	lua_setglobal(L, nameCstr);
	lua_pop(L, 1);
}

void LuauVM::PushFunction(const lua_CFunction& function)
{
	lua_pushcclosurek(L, function, "", 0, nullptr);
}

LuauVM::LuauVM()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	LtoVM[L] = this;
}

LuauVM::~LuauVM()
{
	lua_close(L);
	LtoVM.erase(L);
}

LuauVM* LuauVM::FindVM(lua_State* L)
{
	return LtoVM[L];
}

void LuauVM::UserdataDestructor(void* userdata)
{
	Userdata* userdataPtr = static_cast<Userdata*>(userdata);
	userdataPtr->~Userdata();
}

int LuauVM::IndexFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Index(L);
}

int LuauVM::NewIndexFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->NewIndex(L);
}

int LuauVM::AddFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Add(L);
}

int LuauVM::SubFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Sub(L);
}

int LuauVM::MulFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Mul(L);
}

int LuauVM::DivFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Div(L);
}

int LuauVM::ToStringFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->ToString(L);
}

int LuauVM::ConcatFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Concat(L);
}

int LuauVM::EqFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Eq(L);
}

int LuauVM::LtFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Lt(L);
}

int LuauVM::LeFunction(lua_State* L)
{
	Userdata* userdata = static_cast<Userdata*>(lua_touserdata(L, 1));
	return userdata->Le(L);
}
