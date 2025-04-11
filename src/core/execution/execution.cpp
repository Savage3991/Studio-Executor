//
// Created by savage on 11.04.2025.
//

#include "execution.h"

#include "lapi.h"
#include "lstate.h"
#include "lualib.h"
#include "Luau/Compiler.h"
#include "src/roblox/engine/engine.h"

uintptr_t capabilities = 0xFFFFFFFFFFFFFFFF;

void set_capabilities(Proto *proto, uintptr_t* caps) {
    proto->userdata = caps;
    for (int i = 0; i < proto->sizep; ++i)
        set_capabilities(proto->p[i], caps);
}

void execution::execute(lua_State* rl, std::string code) {
    const char *mutableglobals[] = {
        "Game", "Workspace", "game", "plugin", "script", "shared", "workspace",
        "_G", "_ENV", "_GENV", "_RENV",
        nullptr
    };
    auto compileoptions = Luau::CompileOptions { };
    compileoptions.optimizationLevel = 1;
    compileoptions.debugLevel = 1;
    compileoptions.vectorLib = "Vector3";
    compileoptions.vectorCtor = "new";
    compileoptions.vectorType = "Vector3";
    compileoptions.mutableGlobals = mutableglobals;
    std::string bytecode = Luau::compile(code, compileoptions, {});

    if (bytecode.at(0) == 0) {
        const char* error = bytecode.c_str() + 1;
        engine::print(3, code.c_str());
    }
    else {
        lua_State* L = lua_newthread(rl);
        lua_pop(rl, 1);
        luaL_sandboxthread(L);

        lua_pushcclosure(L, reinterpret_cast<lua_CFunction>(engine::task_defer), "defer", 0);

        L->userdata->identity = 7;
        L->userdata->capabilities = capabilities;

        if (luau_load(L, "SavageSploit-Stu", bytecode.data(), bytecode.size(), 0) != LUA_OK) {
            engine::print(3, "Unknown error occured");
        }

        Closure* cl = clvalue(luaA_toobject(L, -1));

        if (cl) {
            Proto* p = cl->l.p;
            if (p) {
                set_capabilities(p, &capabilities);
            }
        }

        lua_pcall(L, 1,0,0);
    }
}
