
#include <lua.hpp>

#include <unistd.h> // getcwd

#include <iostream>

using namespace std;

int main(int argc, char** argv){
    char buf[256];
    cout << getcwd(buf, sizeof buf - 1) << endl; // 当前工作目录
    
    lua_State *L = luaL_newstate();  /* create state */
    if (L == NULL) {
        cerr << "cannot create state: not enough memory" << endl;
        return EXIT_FAILURE;
    }

    luaopen_base(L); // open the basic library
    luaopen_table(L); // open the table library
    // luaopen_io(L); // open the I/O library
    luaopen_string(L); // open the string lib.
    luaopen_math(L); // open the math lib.

    lua_close(L);

    return 0;
}
