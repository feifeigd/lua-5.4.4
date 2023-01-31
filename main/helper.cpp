#include "helper.h"
#include <cstdarg>
#include <cstdlib>
#include <string.h> // strdup

#ifdef WIN32
char* strsep(char** stringp, char const* delim) {
    if (!stringp)
        return nullptr;
    char* s = *stringp;
    if (!s)
        return nullptr;
    for (auto tok = s;;)
    {
        char c = *s++;
        auto spanp = delim;
        char sc{};
        do
        {
            if ((sc = *spanp++) == c)
            {
                if (c)
                    s[-1] = 0;
                else
                    return nullptr;
                *stringp = s;
                return tok;
            }
        } while (sc);
    }
    return nullptr;
}
#else
#include <sys/errno.h> // errno
#endif


int lua_vnpcall(lua_State* L, char const* table, char const* func, char const* arg, char const* ret, ...){
    

    int pop = 0;
    if(table){
        lua_getfield(L, -1, table);
        if(lua_isnil(L, -1)){
            lua_pushstring(L, "no such table");
            lua_pop(L, 1);
            return LUA_ERRRUN;
        }
        ++pop;
    }
    if (func)
    {
        auto fnam = strdup(func);
        if(! fnam){
            if(pop)
                lua_pop(L, pop);
            lua_pushstring(L, strerror(errno));
            return LUA_ERRMEM;
        }

        auto f = fnam;
        char* t;
        while (( t = strsep(&f, "./"))) // a.b.c
        {
            lua_getfield(L, -1, t);
            if(lua_isnil(L, -1)){
                free(fnam);
                lua_pushstring(L, "no such function");
                if(pop)
                    lua_pop(L, pop);
                return LUA_ERRRUN;
            }
            ++pop;
        }
        free(fnam);
    }
    
    va_list ap;
    va_start(ap, ret);
    int args = 0;
    auto types = arg;
    if(arg){    // 有参数
        for (; *types; ++types)
        {
            switch (*types)
            {
            case 'b':
                lua_pushboolean(L, va_arg(ap, int));
                ++args;
                break;
            case 'f':
                lua_pushnumber(L, va_arg(ap, double));
                ++args;
                break;
            case 'd':
                lua_pushinteger(L, va_arg(ap, int));
                ++args;
                break;
            case 'l':
                lua_pushinteger(L, va_arg(ap, long));
                ++args;
                break;
            case 'L':
                lua_pushinteger(L, va_arg(ap, long long));
                ++args;
                break;
            
            case 's':
                lua_pushstring(L, va_arg(ap, char*));
                ++args;
                break;
            default:
                lua_pop(L, 1 + args + pop);
                lua_pushstring(L, "unknown return type");
                va_end(ap);
                return LUA_ERRRUN;
                break;
            }
        }
    }
    auto rets = ret ? strlen(ret) : 0; // 返回值个数
    if (auto retval = lua_pcall(L, args, rets, 0))
    {
        va_end(ap);
        if(pop)
            lua_pop(L, pop);
        return retval;
    }
    
    if(ret){
        types = ret;
        for (auto n = rets; *types; --n, ++types)
        {
            switch (*types)
            {
            case 'b':
                *(va_arg(ap, int*)) = lua_toboolean(L, -n);
                break;
            case 'f':
                *(va_arg(ap, double*)) = lua_tonumber(L, -n);
                break;
            
            case 'd':
                *(va_arg(ap, int*)) = lua_tointeger(L, -n);
                break;
            case 'l':
                *(va_arg(ap, long*)) = lua_tointeger(L, -n);
                break;
            case 'L':
                *(va_arg(ap, long long*)) = lua_tointeger(L, -n);
                break;

            case 's':
                *(va_arg(ap, char const**)) = lua_tostring(L, -n);
                break;
            default:
                lua_pop(L, lua_gettop(L));
                lua_pushstring(L, "unknown return type");
                va_end(ap);
                return LUA_ERRRUN;
                break;
            }
        }
        if(rets)
            lua_pop(L, rets);
    }
    va_end(ap);
    if(pop)
        lua_pop(L, pop);
    return LUA_OK;
}
