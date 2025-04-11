//
// Created by savage on 10.04.2025.
//

#pragma once
#include <memory>
#include <string>

#include "../engine/engine.h"


class taskscheduler {
public:
    static lua_State* our_State;

    static void initialize_hook(lua_State* L);

    static std::shared_ptr<engine::structures::task_scheduler_job> get_job(std::string);

    static std::vector<std::string> queue;
};
