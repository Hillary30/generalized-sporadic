#include "dbfs.h"

int valid_max_requests(double T, double upper, double lower) { //jobs arrival time and deadline must be between upper and lower
  auto floor_fraction = floor((upper - lower) / T); 
  return max(0, static_cast<int>(floor_fraction));
}

double edf_vd_demand_based_function(double C, double T, double upper, double lower) {
  return C * valid_max_requests(T, upper, lower);
}

double sum_load(const TaskSet& task_set, double t, double ts) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        if(task.L == Level::LO) {
            cout << "LO Task sum_load" << endl;
            sum += edf_vd_demand_based_function(task.C_LO, task.T, t, ts);
        }
        else if(task.L == Level::HI) {
            cout << "HI Task sum_load" << endl;
            sum += edf_vd_demand_based_function(task.C_HI, task.T, t, ts);
        }
    }
    return sum / (t - ts);
}

double sum_load_LO(const TaskSet& task_set, double t, double ts) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        sum += edf_vd_demand_based_function(task.C_LO, task.T, t, ts);
    }
    return sum / (t - ts); 
}

double sum_load_HI(const TaskSet& task_set, double t, double ts) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        if(task.L == Level::HI) {
            sum += edf_vd_demand_based_function(task.C_HI, task.T, t, ts);
        }
    }
    return sum / (t - ts); 
}