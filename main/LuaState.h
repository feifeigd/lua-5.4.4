#pragma once
#include <lua.hpp>
#include <iostream>

void stackDump(lua_State* L);

class LuaState{
public:
    LuaState(lua_State* l = luaL_newstate())
		: L{l}
	{

    }
	~LuaState(){
    	if(L){
			lua_close(L);
    	} 
    }

	operator lua_State*()const{
		return L;
	}

	int dofile(char const* file){
		return luaL_dofile(L, file);
	}

	void push_arg(nullptr_t){
		lua_pushnil(L);
	}
	void push_arg(double arg){
		lua_pushnumber(L, arg);
	}
	void push_arg(char const* arg){
		lua_pushstring(L, arg);
	}
	void push_arg(void* arg){
		lua_pushlightuserdata(L, arg);
	}

	template<typename T>
	typename std::enable_if<std::is_integral<T>::value>::type push_arg(T const& arg){
		lua_pushinteger(L, arg);
	}

	void push_args(){}

	template<typename Arg, typename ... Args>
	void push_args(Arg const& arg, Args const&... args){
		push_arg(arg);
		push_args(args...);
	}

	template<typename ... Args>
	bool call(char const* func, Args const& ...args){
		int old_top = lua_gettop(L);
		lua_getglobal(L, func);
		// 压入参数
		push_args(args...);
		stackDump(L);
		int new_top = lua_gettop(L);
		bool res = true;
		if(lua_pcall(L, new_top - old_top - 1, LUA_MULTRET, 0)){
			std::cerr << "LuaState::call error: " << lua_tostring(L, -1) << std::endl;
			res = false;
		}
		stackDump(L);
		new_top = lua_gettop(L);
		if(res){
			int rets = new_top - old_top;	// lua 函数返回值个数
			for (int i = 0; i < rets; i++)
			{
				int idx = i - rets;
				switch(auto t = lua_type(L, idx)){
				// TODO: 返回值的处理
				default:
					std::cerr << "LuaState::call unhandle value with type: " << t;
					auto s = lua_tostring(L, idx);
					if(s){
						std::cerr << "-" << s;
					}
					std::cerr << std::endl;

					break;
				}
			}
			
		}
		lua_settop(L, old_top); // 清除返回值
		return res;
	}
private:
    lua_State* L{};
};
