#ifndef AMC_H
#define AMC_H

#include <cmath>
#include <vector>

#include "generate_amc_task_set.h"

int lo_crit_busy_period (int q, const AmcTaskSet& task_set, vector<int>& wcrtJobs) {
    for(const auto& [key, task] : task_set.get_task_set()) {
        int busy_period = (q+1) * task.C_LO;
        int sumHigher = 0;
        int answer = -1;
        for (int findBp = task.C_LO; findBp <= task.D; findBp++) {

            for(const auto& [higherPriorityKey, higherPriorityTask] : task_set.get_task_set()) {
                if (higherPriorityTask.priority == -1) {
                    sumHigher += ceil(findBp / higherPriorityTask.T) * higherPriorityTask.C_LO;
                }
            }

            busy_period += sumHigher;
            if (busy_period == findBp) {
                answer = busy_period;
                break;
            }
        }
    }
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