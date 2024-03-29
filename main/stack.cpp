#include <lua.hpp>

// static 
void stackDump(lua_State* L){
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++)
    {
        int t = lua_type(L, i);
        switch (t)
        {
        case LUA_TSTRING:
            printf("`%s'", lua_tostring(L, i));
            break;

        case LUA_TBOOLEAN:
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TNUMBER:
            printf("%g", lua_tonumber(L, i));
            break;
        default:
            printf("%s", lua_typename(L, t));
            break;
        }
        printf(" "); // put a separator
    }
    printf("\n");
}

