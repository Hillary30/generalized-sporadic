#ifndef EDF_VD_H
#define EDF_VD_H


#include "dbfs.h"

int edf_valid_max_requests(double T, double upper, double lower) {
  auto floor_fraction = floor((upper - lower) / T); 
  return max(0, static_cast<int>(floor_fraction));
}

double edf_vd_demand_based_function(double C, double T, double upper, double lower) {
  return C * edf_valid_max_requests(T, upper, lower);
}

double sum_load(const TaskSet& task_set, double t, double ts) {
    double sum = 0;
    for (const auto& [key, task] : task_set.get_task_set()) {
        if(task.C_LO < task.C_HI) {
            sum += edf_vd_demand_based_function(task.C_HI, task.T, t, ts);
        }
        else {
            sum += edf_vd_demand_based_function(task.C_LO, task.T, t, ts);
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
        if(task.C_LO < task.C_HI) {
            sum += edf_vd_demand_based_function(task.C_HI, task.T, t, ts);
        }
    }
    return sum / (t - ts); 
}

bool schedulability_prop_42(double load_lo, double load_hi) {
    if(load_lo > 1 || load_hi > 1) {
        return false; 
    }
    return true;
}

bool schedulability_prop_43(double load, double load_lo, double load_hi) {
    if(load <= (load_lo + load_hi)) {
        return true; 
    }
    return false;
}

bool is_eligible_edf(double load, double load_lo, double load_hi) {
    if(schedulability_prop_42(load_lo, load_hi) == true && schedulability_prop_43(load, load_lo, load_hi) == true) {
        return true;
    }
    return false;
}

bool schedulability_lemma_44(double load) {
    if(load <= 1) {
        return true;
    }
    else {
        return false;
    }
}

bool schedulability_lemma_45(double load_lo) {
    if(load_lo <= 1) {
        return true;
    }
    return false;
}

bool schedulability_lemma_46_p1(double load_lo, double load_hi) {
    if((load_lo + (load_hi/2)) <= 1) {
        return true;
    }
    return false;
}

bool schedulability_lemma_46_p2(double load_lo, double load_hi) {
    if((load_lo + load_hi - (load_lo*load_hi/4)) <= 1) {
        return true;
    }
    return false;
}

bool schedulability_lemma_46(double load_lo, double load_hi) {
    if(schedulability_lemma_46_p1(load_lo, load_hi) == true && schedulability_lemma_46_p2(load_lo, load_hi) == true) {
        return true; 
    }
    return false; 
}


bool offline_pp(TaskSet& task_set, double t, double ts) {
    double load = sum_load(task_set, t, ts);
    double load_LO = sum_load_LO(task_set, t, ts);
    double load_HI = sum_load_HI(task_set, t, ts);

    if(is_eligible_edf(load, load_LO, load_HI) == false) {
        return false;
    }
    if(schedulability_lemma_44(load) == true) {
        task_set.opp_klevel = 2;
        for (auto& [key, task] : task_set.get_task_set()) {
            task.tight_D = task.D;
        }
    }
    else {
        double x = 1 - (load_HI/2);
        if(schedulability_lemma_46(load_LO, load_HI) == true) {
            task_set.opp_klevel = 1;
            for (auto& [key, task] : task_set.get_task_set()) {
                if(task.L == HI) {
                    task.tight_D = task.D * x;
                }
                else { 
                    task.tight_D = task.D;
                }
            }
        }
        else {
            return false;
        }
    }
    return true;
}


#endif