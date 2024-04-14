#include "dbfs.h"

double sum_load(const TaskSet& task_set) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        if(task.L == Level::LO) {
            sum += task.C_LO;
        }
        else if(task.L == Level::HI) {
            sum += task.C_HI;
        }
    }
    return sum; //needs to be divided by time interval given that task job is inside the interval
}

double sum_load_1(const TaskSet& task_set) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        sum += task.C_LO;
    }
    return sum; //needs to be divided by time interval
}

double sum_load_2(const TaskSet& task_set) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        if(task.L == Level::HI) {
            sum += task.C_HI;
        }
    }
    return sum; //needs to be divided by time interval
}