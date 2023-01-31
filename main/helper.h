#pragma once

#include <lua.hpp>

/// @param arg/ret 函数参数类型 b bool, f double, d int, l long, L long long, s char*
int lua_vnpcall(lua_State* L, char const* table, char const* func, char const* arg, char const* ret, ...);
