#ifndef SEARCH_ALGORITHM_H
#define SEARCH_ALGORITHM_H

#include <deque>
#include <algorithm>
#include <string>
#include <queue>

#include "schedulability.h"
#include "edf_vd.h"

bool is_eligible(TaskSet& task_set) {
  return !task_set.get_thm1() && task_set.get_thm2() && task_set.get_thm3();
}

deque<int> get_best_candidates(TaskSet& task_set) {
  vector<Task> hi_tasks;

  for (const auto& [key, task] : task_set.get_task_set()) {
    if (task.L == HI) {
      hi_tasks.push_back(task);
    }
  }

  sort(hi_tasks.begin(), hi_tasks.end(), [](const Task& a, const Task& b) {
    return (a.C_HI - a.C_LO) > (b.C_HI - b.C_LO);
  });

  deque<int> candidates;
  for (const auto& task : hi_tasks) {
    candidates.push_back(task.ID);
  }

  return candidates;
}

deque<int> get_hi_candidates(TaskSet& task_set) {
  deque<int> candidates;
  for (const auto& [key, task] : task_set.get_task_set()) {
    if (task.L == HI) {
      candidates.push_back(task.ID);
    }
  }
  return candidates;
}


pair<int, int> get_failure_time(TaskSet& task_set) {
  for (int i = 2; i < task_set.get_t_max() + 1; ++i) {
    for (int j = 1; j < i; ++j) {
      if (sum_dbf(task_set, i, j) > i) { 
        return {i, j};
      }
    }
  }
  return {-1, -1};
}


bool is_demand_at_minimum(int T, int C_LO, int D, int ts) {
  return ts < C_LO || ts - floor((ts - 1) / T) * T > D;
}

int find_optimal_tight_D(int T, int C_LO, int previous_tight_D, int ts) {
  for (int n = 0; n <= (ts - 1) / T + 1; ++n) {
    if (C_LO <= ts - n * T && ts - n * T < previous_tight_D) { //tightened deadline = ts - n * T
      return ts - n * T;
    }
  }
  return previous_tight_D;
}

// Enhanced Deadline Search Algorithm
string deadline_search_algorithm(TaskSet& task_set) {
  if (!is_eligible(task_set)) return "Not eligible for deadline-tightening";

  deque<int> candidates = get_best_candidates(task_set); //EDS orders the tasks by greatest margin of C_HI-C_LO
  int best_candidate = -1;
  auto [t, ts] = get_failure_time(task_set);

  while (!candidates.empty()) {
    best_candidate = candidates.front();
    candidates.pop_front();

    int T = task_set.task_set[best_candidate].T;
    int C_LO = task_set.task_set[best_candidate].C_LO;
    int D = task_set.task_set[best_candidate].D;
    int previous_tight_D = task_set.task_set[best_candidate].tight_D;

    if (is_demand_at_minimum(T, C_LO, D, ts)) {
      continue;
    }

    task_set.task_set[best_candidate].tight_D = find_optimal_tight_D(T, C_LO, previous_tight_D, ts);
    if (task_set.task_set[best_candidate].tight_D == previous_tight_D) {
      continue;
    }

    if (!schedulability_test_thm2_3(task_set)) {
      task_set.task_set[best_candidate].tight_D = previous_tight_D;
      continue;
    }

    auto [t, ts] = get_failure_time(task_set);
    if (t == -1 && ts == -1) {
      task_set.set_thm1(true);
      return "Success";
    }

    candidates.push_front(best_candidate);
  }

  return "No more eligible candidates";
}

// Naive Algorithm
string naive_algorithm(TaskSet& task_set) {
  if (!is_eligible(task_set)) return "Not eligible for deadline-tightening";

  deque<int> candidates = get_hi_candidates(task_set); //Naive selects HI tasks at random
  int best_candidate = -1;
  //auto [t, ts] = get_failure_time(task_set);

  while (!candidates.empty()) { // while there are still tasks
    best_candidate = candidates.front(); //get the task that arrived first
    candidates.pop_front();

    int C_LO = task_set.task_set[best_candidate].C_LO; //first task execution
    int previous_tight_D = task_set.task_set[best_candidate].tight_D; //store the first task tightended deadline

    task_set.task_set[best_candidate].tight_D -= 1; //subtract 1 time unit from the tight deadline
    
    if (task_set.task_set[best_candidate].tight_D < C_LO) { //if miss deadline
      task_set.task_set[best_candidate].tight_D = previous_tight_D;
      continue;
    }
    
    if (!schedulability_test_thm2_3(task_set)) {
      task_set.task_set[best_candidate].tight_D = previous_tight_D;
      continue;
    }

    auto [t, ts] = get_failure_time(task_set);
    if (t == -1 && ts == -1) {
      task_set.set_thm1(true);
      return "Success";
    }

    candidates.push_front(best_candidate);
  }

  return "No more eligible candidates";
}


string edf_vd_algorithm(TaskSet& task_set) {
  bool can_schedule_offline_pp = offline_pp(task_set, 500, 25);

  if(can_schedule_offline_pp == true) {
    return "Success";
  }
  else if(can_schedule_offline_pp == false) {
    return "Not eligible for deadline-tightening";
  }

  return "Fail";
}


#endif

//EDF or EDF-VD
// //runtime scheduling
// deque<int> get_sort_hi_candidates(TaskSet& task_set) {
//   vector<Task> hi_tasks;

//   for (const auto& [key, task] : task_set.get_task_set()) {
//     if (task.L == HI) {
//       hi_tasks.push_back(task);
//     }
//   }

//   sort(hi_tasks.begin(), hi_tasks.end(), [](const Task& a, const Task& b) {
//     return a.tight_D > b.tight_D;
//   });

//   deque<int> candidates;
//   for (const auto& task : hi_tasks) {
//     candidates.push_back(task.ID);
//   }

//   return candidates;
// }

// // Comparator for priority queue ordering by deadline (smallest to greatest)
// struct CompareByDeadline {
//     bool operator()(const Task& t1, const Task& t2) const {
//         return t1.absolute_deadline > t2.absolute_deadline; // Priority by ascending deadline
//     }
// };

// // Comparator for priority queue ordering by virtual deadline (smallest to greatest)
// struct CompareByVirtualDeadline {
//     bool operator()(const Task& t1, const Task& t2) const {
//         return t1.virtual_deadline > t2.virtual_deadline; // Priority by ascending virtual deadline
//     }
// };

// string edf_vd_algorithm(TaskSet& task_set) {
//   bool can_schedule_offline_pp = offline_pp(task_set, 500, 25);
//   // auto [t, ts] = get_failure_time(task_set);
//   // if (t == -1 && ts == -1) {
//   //   task_set.set_thm1(true);
//   //   cout << "Pass Fail Time" << endl;
//   // }
//   // else {
//   //   cout << "Fail Fail Time" << endl;
//   // }

//   if(can_schedule_offline_pp == true) {
    

//     // cout << "Pass OPP" << endl;
//     // deque<int> candidates = get_sort_hi_candidates(task_set); //sorted by tightdeadline

//     // int currentTime = 0;
//     // priority_queue<Task, vector<Task>, CompareByDeadline> deadline_queue;
//     // priority_queue<Task, vector<Task>, CompareByVirtualDeadline> virtual_deadline_queue;

//     // while(!candidates.empty() || !virtual_deadline_queue.empty()) {
//     //   if(currentTime >= 1000) {
//     //     break;
//     //   }
//     //   int taskIndex = -1;
//     //   if(!candidates.empty()) {
//     //     taskIndex = candidates.front();//check if not empty
//     //     candidates.pop_front();
//     //     Task candidate = task_set.task_set[taskIndex];
//     //     candidate.arrival_time = currentTime;
//     //     candidate.absolute_deadline = candidate.D + currentTime;
//     //     candidate.virtual_deadline = candidate.tight_D + currentTime;

//     //     //Dont change values below
//     //     virtual_deadline_queue.push(candidate);
//     //     if(candidate.L == Level::HI) {
//     //       deadline_queue.push(candidate);
//     //     }
//     //   }

//     //   Task top_task = virtual_deadline_queue.top();
//     //   virtual_deadline_queue.pop();

//     //   //remove task from deadlineQ
//     //   priority_queue<Task, vector<Task>, CompareByDeadline> tempQueue;
//     //   while(!deadline_queue.empty()) {
//     //     Task current = deadline_queue.top();
//     //     deadline_queue.pop();
//     //     if(current == top_task) {
//     //       continue;
//     //     }
//     //     else {
//     //       tempQueue.push(current);
//     //     }
//     //   }
//     //   deadline_queue = tempQueue;

//     //   //Dont change values above

//     //   if(top_task.tight_D < top_task.C_LO || top_task.virtual_deadline < currentTime) {
//     //     cout << "Missed Deadline, switch to HI" << endl;
//     //     currentTime += top_task.C_HI;
//     //     top_task.virtual_deadline += top_task.arrival_time + top_task.T + top_task.tight_D;
//     //     top_task.absolute_deadline += top_task.arrival_time + top_task.T + top_task.D;

//     //     deadline_queue.push(top_task);
//     //     break;
//     //   }
//     //   else {
//     //     cout << "No Miss" << endl;
//     //     currentTime += top_task.C_LO;
//     //     top_task.virtual_deadline += top_task.arrival_time + top_task.T + top_task.tight_D;
//     //     top_task.absolute_deadline += top_task.arrival_time + top_task.T + top_task.D;
//     //     virtual_deadline_queue.push(top_task);
//     //     deadline_queue.push(top_task);
//     //   }
//     // }
    
//     // while(!candidates.empty() || !deadline_queue.empty()) {
//     //   if(currentTime >= 1000) {
//     //     break;
//     //   }
//     //   cout << "HI Task execute C_HI" << endl;
//     //   int taskIndex = -1;
//     //   Task task;
//     //   if(!candidates.empty()) {
//     //     taskIndex = candidates.front();
//     //     candidates.pop_front();
//     //     task = task_set.task_set[taskIndex];
//     //     task.absolute_deadline = currentTime + task.D;
//     //     deadline_queue.push(task);
//     //   }
//     //   Task priority_task = deadline_queue.top();
//     //   deadline_queue.pop();

//     //   currentTime += priority_task.C_HI;
//     //   if(priority_task.tight_D < priority_task.C_LO || priority_task.virtual_deadline < currentTime) {
//     //     cout << "Missed Deadline, switch to HI" << endl;
//     //     currentTime += priority_task.C_HI;
//     //     priority_task.virtual_deadline += priority_task.arrival_time + priority_task.T + priority_task.tight_D;
//     //     priority_task.absolute_deadline += priority_task.arrival_time + priority_task.T + priority_task.D;

//     //     deadline_queue.push(priority_task);
//     //     break;
//     //   }
//     //   else {
//     //     cout << "No Miss" << endl;
//     //     currentTime += priority_task.C_HI;
//     //     priority_task.virtual_deadline += priority_task.arrival_time + priority_task.T + priority_task.tight_D;
//     //     priority_task.absolute_deadline += priority_task.arrival_time + priority_task.T + priority_task.D;
//     //     deadline_queue.push(priority_task);
//     //   }
//     // }
//     return "Success";
//   }
//   else if(can_schedule_offline_pp == false) {
//     return "Not eligible for deadline-tightening";
//   }

//   return "Fail";
// }

