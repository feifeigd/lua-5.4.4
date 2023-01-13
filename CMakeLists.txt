
cmake_minimum_required(VERSION 3.2)

project(lua)

aux_source_directory(src SRC)
remove(SRC src/lua.c src/luac.c) 
message(STATUS ${SRC})
add_definitions(-DLUA_BUILD_AS_DLL)
add_library(luadll  ${SRC})	# dll
if(UNIX)
    target_link_libraries(luadll m) # 数学库
endif()
remove_definitions(-DLUA_BUILD_AS_DLL)
add_executable(lua src/lua.c)	# 解析器
target_link_libraries(lua luadll)

add_executable(luac src/luac.c)	# 编译器
target_link_libraries(luac luadll)
