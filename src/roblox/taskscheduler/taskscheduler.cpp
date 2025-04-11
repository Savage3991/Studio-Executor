//
// Created by savage on 10.04.2025.
//

#include "taskscheduler.h"

#include "lua.h"
#include "src/core/execution/execution.h"

std::vector<std::string> taskscheduler::queue = {};
lua_State* taskscheduler::our_State = nullptr;

int step(lua_State* L) {
    if (!taskscheduler::queue.empty()) {
        //printf("Detected Script :P");

        execution::execute(taskscheduler::our_State, *taskscheduler::queue.begin());

        taskscheduler::queue.erase(taskscheduler::queue.begin());
    }

    return 0;
}

void taskscheduler::initialize_hook(lua_State *L) {
    taskscheduler::our_State = L;

    lua_pushcclosure(L, step, nullptr, 0);
    lua_setglobal(L, "um_dont_call_this_or_bad_things_will_happen");

    execution::execute(L, "game:GetService('RunService').Heartbeat:Connect(function() um_dont_call_this_or_bad_things_will_happen() end)");
}


std::shared_ptr<engine::structures::task_scheduler_job> taskscheduler::get_job(std::string job_name) {

    const auto task_scheduler = *reinterpret_cast< engine::structures::task_scheduler** >(engine::taskscheduler);

    for (const auto &job : task_scheduler->jobs) {
        if (strcmp(job->name.c_str(), job_name.c_str()) == 0) {

            if (job_name == "WaitingHybridScriptsJob") {
                const auto datamodel = job->fake_datamodel.datamodel;

                if (datamodel->name != "Game")
                    return job;
            }
            else {
                return job;
            }

        }
    }

    return {};

}
