//
// Created by savage on 11.04.2025.
//
#include "../environment.h"

int identifyexecutor(lua_State* L) {

    lua_pushstring(L, "SavageSploit-Stu");
    lua_pushstring(L, "0.1");

    return 2;
}

void environment::initialize_misc(lua_State *L) {

    lua_pushcclosure(L, identifyexecutor, nullptr, 0);
    lua_setglobal(L, "identifyexecutor");
    lua_pushcclosure(L, identifyexecutor, nullptr, 0);
    lua_setglobal(L, "getexecutorname");

}
