#pragma once
#include "lualib.h"
#include "iostream"
#include "LuauVM.h"

class LuauVM;

class Userdata {
public:
    virtual int Index(lua_State* L);
    virtual int NewIndex(lua_State* L);
    virtual ~Userdata();
};