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
        return false; //not schedulable
    }
    return true;
}

bool schedulability_prop_43(double load, double load_lo, double load_hi) {
    if(load <= (load_lo + load_hi)) {
        return true; //schedulable
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

bool schedulability_lemma_46(double load_lo, double load_hi) {
    if(((load_lo + (load_hi/2)) <= 1) && ((load_lo + load_hi - (load_lo*load_hi/4)) <= 1)){
        return true; //it is schedulable
    }
    return false; //it is unschedulable
}


bool offline_pp(TaskSet& task_set, double t = 500, double ts = 0) {
    double load = sum_load(task_set, t, ts);
    double load_LO = sum_load_LO(task_set, t, ts);
    double load_HI = sum_load_HI(task_set, t, ts);

    if(is_eligible_edf(load, load_LO, load_HI) == false) {
        //cout << "Fail is_eligible" << endl;
        return false;
    }
    if(schedulability_lemma_44(load) == true) {
        task_set.opp_klevel = 2; //HI --> Normal EDF
        task_set.set_tightd_eq_deadline();
        //cout << "K Level 2" << endl;
    }
    else {
        if(schedulability_lemma_46(load_LO, load_HI) == true) {
            //cout << "Pass Lemma 46" << endl;
            task_set.opp_klevel = 1; //LO
            double x = 1 - (load_HI/2);
            for (auto& [key, task] : task_set.get_task_set()) {
                if(task.C_LO < task.C_HI) { //scale HI task
                    task.tight_D = task.D * x;
                    
                }
                else { 
                    task.tight_D = task.D;
                }
            }
        }
        else /*if(schedulability_lemma_46(load_LO, load_HI) == false) */{ //if unschedulable
            //cout << "Fail Lemma 46" << endl;
            return false; //return unschedulable
        }
    }
    return true;
}


#endif