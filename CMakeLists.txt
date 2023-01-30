
cmake_minimum_required(VERSION 3.2)

project(lua)

set(CMAKE_CXX_STANDARD 11) # sol2 最低要 c++17

aux_source_directory(src SRC)
remove(SRC src/lua.c src/luac.c) 
message(STATUS ${SRC})
add_library(lua_static ${SRC})	# lua 静态库
add_definitions(-DLUA_BUILD_AS_DLL)
add_library(luadll SHARED ${SRC})	# dll
target_include_directories(luadll PUBLIC src)
if(UNIX)
    target_link_libraries(luadll m) # 数学库
endif()
remove_definitions(-DLUA_BUILD_AS_DLL)
add_executable(lua src/lua.c)	# 解析器
target_link_libraries(lua luadll)

add_executable(luac src/luac.c)	# 编译器
target_link_libraries(luac luadll)

include_directories(${CMAKE_SOURCE_DIR}/3rd/sol2/include)
add_subdirectory(main)
