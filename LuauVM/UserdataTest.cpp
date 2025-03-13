#include "UserdataTest.h"

UserdataTest::UserdataTest(int number)
{
	num = number;
}

UserdataTest::~UserdataTest()
{
	std::cout << "userdata was destroyed!";
}

int UserdataTest::Call(lua_State* L)
{
	UserdataTest* userdata = static_cast<UserdataTest*>(lua_touserdata(L, 1));

	if (userdata)
	{
		std::cout << "this was called by luau! i got value " << userdata->num << "\n";
	}

	return 0;
}

int UserdataTest::New(lua_State* L)
{
	LuauVM* vm = LuauVM::FindVM(L);
	int number = 0;

	if (lua_isnumber(L, 1))
	{
		number = (int)lua_tonumber(L, 1);
	}
	vm->PushUserdata<UserdataTest>(10);
	return 1;
}

int UserdataTest::Register(LuauVM &vm)
{
	lua_newtable(vm.L);
	vm.PushFunction(New);
	lua_setfield(vm.L, -2, "new");
	lua_setglobal(vm.L, "test");
	return 0;
}

int UserdataTest::Index(lua_State* L)
{
	LuauVM* vm = LuauVM::FindVM(L);
	std::string name = std::string(lua_tostring(L, 2));

	if (name == "call")
	{
		vm->PushFunction(Call);
		return 1;
	}

	if (name == "num")
	{
		lua_pushinteger(L, num);
		return 1;
	}

	return 0;
}

int UserdataTest::NewIndex(lua_State* L)
{
	std::string name = std::string(lua_tostring(L, 2));

	if (name == "num")
	{
		if (!lua_isnumber(L, 3))
		{
			return 0;
		}

		double number = lua_tonumber(L, 3);
		num = (int)number;
	}
	return 0;
}
