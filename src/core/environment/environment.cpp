//
// Created by savage on 11.04.2025.
//

#include "environment.h"

void environment::initialize(lua_State *L) {

    lua_newtable(L);
    lua_setglobal(L, "_G");

    lua_newtable(L);
    lua_setglobal(L, "shared");

    environment::initialize_misc(L);


}
