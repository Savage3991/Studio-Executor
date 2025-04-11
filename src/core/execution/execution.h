//
// Created by savage on 11.04.2025.
//

#pragma once
#include <string>
#include "lua.h"

class execution {
public:
    static void execute(lua_State*, std::string);
};
