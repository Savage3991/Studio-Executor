//
// Created by savage on 09.04.2025.
//

#pragma once

#include "Windows.h"
#include <vector>
#include <memory>
#include <string>

struct lua_State;

inline uintptr_t rebase(uintptr_t x) {
    return x + reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
}

namespace engine {

    namespace structures {

        struct extra_space {
            char pad_1[ 0x30 ];
            uint32_t identity; // 0x30

            char pad_2[ 0x10 ];
            uintptr_t capabilities; // 0x48
        };

        struct class_descriptor {
            void* vftable; // 0x0
            std::string &class_name; // 0x8
        };

        struct instance {
            void* vftable; // 0x0
            char pad_1[ 0x10 ];
            class_descriptor &class_descriptor; // 0x18

            char pad_2[ 0x58 ];
            std::string &name; // 0x78
            std::vector<std::shared_ptr<instance>> &children; // 0x80
        };

        struct datamodel final : instance {};

        struct fake_datamodel {
            char pad[ 0x1a8 ];
            std::shared_ptr<datamodel> datamodel; // 0x1a8
        };

        struct task_scheduler_job {
            void* vftable; // 0x0
            char pad[ 0x10 ];
            std::string name; // 0x18

            fake_datamodel &fake_datamodel; // 0x38
        };

        struct task_scheduler {
            char pad[ 0x1D0 ];
            std::vector<std::shared_ptr<task_scheduler_job>> jobs; // 0x1d0
        };

    }

    inline auto taskscheduler = rebase(0xA13E2B0);

    inline auto print = reinterpret_cast<uintptr_t(__fastcall*)(int type, const char* message, ...)>(rebase(0x2448140));

    inline auto get_global_state = reinterpret_cast<lua_State*(__fastcall*)(uintptr_t script_context, uintptr_t* identity, uintptr_t* base_instance )>(rebase(0x1F21060));

    inline auto task_defer = rebase(0x23AE0A0);

}
