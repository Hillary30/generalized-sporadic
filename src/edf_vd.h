#ifndef EDF_VD_H
#define EDF_VD_H


#include "dbfs.h"

int edf_valid_max_requests(double T, double upper, double lower) { //jobs arrival time and deadline must be between upper and lower
  auto floor_fraction = floor((upper - lower) / T); 
  return max(0, static_cast<int>(floor_fraction));
}

double edf_vd_demand_based_function(double C, double T, double upper, double lower) {
  return C * edf_valid_max_requests(T, upper, lower);
}

double sum_load(const TaskSet& task_set, double t, double ts) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        if(task.L == Level::LO) {
            sum += edf_vd_demand_based_function(task.C_LO, task.T, t, ts);
        }
        else if(task.L == Level::HI) {
            sum += edf_vd_demand_based_function(task.C_HI, task.T, t, ts);
        }
    }
    return sum / (t - ts);
    // return sum;
}

double sum_load_LO(const TaskSet& task_set, double t, double ts) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        sum += edf_vd_demand_based_function(task.C_LO, task.T, t, ts);
    }
    return sum / (t - ts); 
    // return sum;
}

double sum_load_HI(const TaskSet& task_set, double t, double ts) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        if(task.L == Level::HI) {
            sum += edf_vd_demand_based_function(task.C_HI, task.T, t, ts);
        }
    }
    return sum / (t - ts); 
    // return sum; 
}


bool schedulability_lemma_44(TaskSet& task_set, double t, double ts) {
    if(sum_load(task_set, t, ts)<= 1) {
        return true;
    }
    else {
        return false;
    }
}

bool schedulability_lemma_46(double load_lo, double load_hi) {
    if(((load_lo + (load_hi/2)) <= 1) && ((load_lo + load_hi - (load_lo*load_hi/4)) <= 1)){
        return true; //it is schedulable
    }
    return false; //it is unschedulable
}


bool offline_pp(TaskSet& task_set, double t = 500, double ts = 0) {
    if(schedulability_lemma_44(task_set, t, ts) == true) {
        task_set.opp_klevel = 2; //HI --? Normal EDF
        task_set.set_tightd_eq_deadline();
    }
    else {
        double load_LO = sum_load_LO(task_set, t, ts); //load1
        double load_HI = sum_load_HI(task_set, t, ts); //load2

        if(schedulability_lemma_46(load_LO, load_HI) == false) { //if unschedulable
            return false; //return unschedulable
        }
        else {
            task_set.opp_klevel = 1; //LO
            double x = 1 - load_HI/2;
            for (auto& [key, task] : task_set.get_task_set()) {
                if(task.L == Level::LO) {
                    task.tight_D = task.D;
                }
                else { //scale HI task
                    task.tight_D = task.D * x;
                }
            }
        }
    }
    return true;
}





#endif