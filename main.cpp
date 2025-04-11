#include <thread>
#include <iostream>
#include <istream>
#include <string>
#include <windows.h>

#include "lstate.h"
#include "lualib.h"
#include "src/roblox/engine/engine.h"
#include "src/roblox/taskscheduler/taskscheduler.h"

#include "Luau/Compiler.h"
#include "Luau/Bytecode.h"
#include "Luau/BytecodeUtils.h"
#include "Luau/BytecodeBuilder.h"
#include "src/core/environment/environment.h"
#include "src/core/execution/execution.h"

int main() {

    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    printf("HIII!!!!!!\n");

    const auto whsj = taskscheduler::get_job("WaitingHybridScriptsJob");
    const auto datamodel = whsj->fake_datamodel.datamodel;
    printf("DataModel->Name: %s\n", datamodel->name.c_str());

    std::shared_ptr<engine::structures::instance> script_context;

    for (const auto &child : datamodel->children) {
        if (child->class_descriptor.class_name == "ScriptContext") {
            script_context = child;
            break;
        }
    }

    printf("ScriptContext->Name: %s\n\n", script_context->name.c_str());

    uintptr_t identity = 0;
    uintptr_t base_instance = 0;
    const auto global_state = engine::get_global_state(reinterpret_cast<uintptr_t>(script_context.get()),&identity,&base_instance);
    printf("ScriptContext-Global_State: %p\n", global_state);

    lua_State* our_state = lua_newthread(global_state);
    lua_ref(global_state, -1);

    printf("state: %p\n", our_state);

    luaL_sandboxthread(our_state);

    taskscheduler::initialize_hook(our_state);

    environment::initialize(our_state);

    //our_state->userdata->identity.identity = 8;
    //our_state->userdata->capabilities = capabilities;

    while (true) {
        std::string script;
        printf("Enter Script:");
        std::getline(std::cin,script);
        printf("\n");

        taskscheduler::queue.push_back(script);

    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            std::thread(main).detach();
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
