#ifndef AMC_H
#define AMC_H

#include <cmath>
#include <vector>
#include <algorithm>

#include "generate_task_set.h"

int compute_busy_period_lo (int q, const TaskSet& task_set, int currentKey) {
    Task currentTask = task_set.get_task_set().at(currentKey);
    int busyPeriod = (q+1) * currentTask.C_LO;

    for (int findBp = busyPeriod; findBp <= currentTask.D + (q*currentTask.T); findBp++) {
        int sumHigher = 0;

        for(const auto& [higherPriorityKey, higherPriorityTask] : task_set.get_task_set()) {
            if (higherPriorityTask.priority == -1 && higherPriorityKey != currentKey) {
                sumHigher += ceil((float) findBp / higherPriorityTask.T) * higherPriorityTask.C_LO;
            }
        }

        if ((busyPeriod + sumHigher) == findBp) {
            return findBp;
        }
    }

    return -1;
}

int compute_response_time_lo (const TaskSet& task_set, int currentKey, int& lastReleaseLO) {
    Task currentTask = task_set.get_task_set().at(currentKey);
    vector<int> wcrtJobs;
    int p = 0;
    while (true) {
        int jobBusyPeriod = compute_busy_period_lo(p, task_set, currentKey);

        if (jobBusyPeriod != -1) {
            int response_time_job_lo = jobBusyPeriod - (p * currentTask.T);
            wcrtJobs.push_back(response_time_job_lo);
        } else {
            return jobBusyPeriod;
        }

        if (jobBusyPeriod <= (p + 1) * currentTask.T) {
            int response_time_task_lo = *max_element(wcrtJobs.begin(), wcrtJobs.end());
            lastReleaseLO = p;
            return response_time_task_lo;
        } else {
            p += 1;
        }
    }
}

int compute_busy_period_hi (int q, const TaskSet& task_set, int currentKey, int lastReleaseLO) {
    Task currentTask = task_set.get_task_set().at(currentKey);
    int busyPeriodHI = (q+1) * currentTask.C_HI;
    int busyPeriodLO = 0;

    if (q < lastReleaseLO) {
        busyPeriodLO = compute_busy_period_lo(q, task_set, currentKey);
    } else {
        busyPeriodLO = compute_busy_period_lo(lastReleaseLO, task_set, currentKey);
    }

    for (int findBp = busyPeriodHI; findBp <= currentTask.D + (q*currentTask.T); findBp++) {
        int sumHigherLO = 0;
        int sumHigherHI = 0;

        for(const auto& [higherPriorityKey, higherPriorityTask] : task_set.get_task_set()) {
            if (higherPriorityTask.priority == -1 && higherPriorityTask.L == HI && higherPriorityKey != currentKey) {
                sumHigherHI += ceil((float) findBp / higherPriorityTask.T) * higherPriorityTask.C_HI;
            }

            if (higherPriorityTask.priority == -1 && higherPriorityTask.L == LO && higherPriorityKey != currentKey) {
                sumHigherLO += ceil((float) busyPeriodLO / higherPriorityTask.T) * higherPriorityTask.C_LO;
            }
        }

        if ((busyPeriodHI + sumHigherHI + sumHigherLO) == findBp) {
            return findBp;
        }
    }

    return -1;
}

int compute_response_time_hi (const TaskSet& task_set, int currentKey, int lastReleaseLO) {
    Task currentTask = task_set.get_task_set().at(currentKey);
    vector<int> wcrtJobs;
    int v = 0;
    while (true) {
        int jobBusyPeriod = compute_busy_period_hi(v, task_set, currentKey, lastReleaseLO);

        if (jobBusyPeriod != -1) {
            int response_time_job_hi = jobBusyPeriod - (v * currentTask.T);
            wcrtJobs.push_back(response_time_job_hi);
        } else {
            return jobBusyPeriod;
        }

        if (jobBusyPeriod <= (v + 1) * currentTask.T) {
            int response_time_task_hi = *max_element(wcrtJobs.begin(), wcrtJobs.end());
            return response_time_task_hi;
        } else {
            v += 1;
        }
    }
}

bool amc_schedulability_test (const TaskSet& task_set, int currentKey) {
    int lastJobReleased = 0;
    Task currentTask = task_set.get_task_set().at(currentKey);
    int responseTimeLO = compute_response_time_lo(task_set, currentKey, lastJobReleased);
    int responseTimeHI = compute_response_time_hi(task_set, currentKey, lastJobReleased);

    if (responseTimeLO <= currentTask.D && responseTimeHI <= currentTask.D && responseTimeLO != -1 && responseTimeHI != -1) {
        return true;
    }
    return false;
}

bool audsleys_optimal_priorirty_assignment (TaskSet& task_set) {
    int n = task_set.get_num_tasks();
    bool failed = false;
    // bool finished;
    while (n != 1 && failed != true) {
        failed = true;
        // finished = false;
        for(auto& [key, task] : task_set.get_task_set_ref()) {
            if (task.priority == -1) {
                task.priority = n;
                bool taskScedulable = amc_schedulability_test(task_set, key);
                if (taskScedulable) {
                    n -= 1;
                    failed = false;
                    // finished = true;
                    break;
                } else {
                    task.priority = -1;
                }
            }
        }
    }

    if (n == 1) {
        return true;
    }
    return false;
}

#endif

// Audsley Algorithm 

// n = num tasks
// while n != 1 and NOT failed
//      finished = false
//      failed = true
//      for every unscheduled task (j) in task set        // Looping over all tasks in task set
//          set task j to priority = n
//          if passes schedulability test
//              n = n -1
//              failed = false
//              finished = true
//          else
//              j priority = -1
//          if finished
//              break;
//
// if (n == 1)
//      return schedulable
// else
//      return unschedulable