#ifndef GENERATE_AMC_TASK_SET_H
#define GENERATE_AMC_TASK_SET_H
 
#include "generate_amc_tasks.h"

class AmcTaskSet {
public:
  map<int, AmcTask> task_set;
  int num_tasks;
  double utilization; //, t_max;
  // bool thm1, thm2, tm3;

  AmcTaskSet() {}

  AmcTaskSet(double target_u, double hi_probability = 0.5, int wcet_ratio = 4) {
    this->task_set = {};
    this->num_tasks = 0;
    this->lo_tasks_list = {};
    this->hi_tasks_list = {};

    while (true) {
      AmcTask task = generate_amc_task(this->num_tasks, hi_probability, wcet_ratio);

      this->task_set[this->num_tasks] = task;
      this->num_tasks = this->task_set.size();
      this->utilization = this->calculate_utilization();

      if (task.L == Level::LO) {
        this->lo_tasks_list.push_back(task);
      }

      if (task.L == Level::HI) {
        this->hi_tasks_list.push_back(task);
      }

      if ((target_u - 0.005 <= this->utilization && this->utilization <= target_u + 0.005)
        && !this->lo_tasks_list.empty() && !this->hi_tasks_list.empty()) {
        break;
      }

      else if (this->utilization > target_u + 0.005) {
        this->task_set.clear();
        this->num_tasks = 0;
        this->lo_tasks_list.clear();
        this->hi_tasks_list.clear();
      }
    }
  } 

  AmcTaskSet(map<string, vector<AmcTask>> task_set_dict) {
    this->lo_tasks_list = task_set_dict["lo"];
    this->hi_tasks_list = task_set_dict["hi"]; 
    
    for (AmcTask task : this->lo_tasks_list) {
      this->task_set[task.ID] = task;
    }

    for (AmcTask task : this->hi_tasks_list) {
      this->task_set[task.ID] = task;
    }

    this->num_tasks = this->task_set.size();
    this->utilization = calculate_utilization();
  }

  AmcTaskSet& operator=(const AmcTaskSet& other) {
    if (this != &other) {
      this->num_tasks = other.num_tasks;
      this->utilization = other.utilization;

      for (const auto& [key, task] : other.get_task_set()) {
        AmcTask copied_task = task;
        this->task_set[key] = copied_task;
      }
    }
    return *this;
  }

  bool operator==(const AmcTaskSet& other) const {
    if (this == &other) return true; 

    for (auto& [key, task] : task_set) {
      if (task == other.get_task_set()[key]) {
        continue;
      } else return false;
    }

    return this->num_tasks == other.num_tasks &&
           this->utilization == other.utilization;
  }

  double calculate_utilization() {
    double lo_utilization = 0;
    double hi_utilization = 0;

    for(auto const&[key, task]: task_set) {
      
      if (task.L == Level::LO) { // Calculate the sum of C_LO / T for all tasks with level LO
        lo_utilization += task.C_LO / static_cast<double>(task.T);
      }

      
      if (task.L == Level::HI) { // Calculate the sum of C_HI / T for all tasks with level HI
        hi_utilization += task.C_HI / static_cast<double>(task.T);
      }
    }

    return (lo_utilization + hi_utilization) / 2;
  }

  string task_set_to_string() {
    string task_set_string = "Task set: " + to_string(num_tasks) + ", utilization = " + to_string(utilization) + "\n";

    for(auto &[key, task]: task_set) {
      task_set_string += task.amc_task_to_string() + "\n";
    }

    return task_set_string;
  }

  void set_utilization(double utilization) { this->utilization = utilization; }
  void set_hi_probability(double hi_probability) { this->hi_probability = hi_probability; }
  void set_num_tasks(int num_tasks) { this->num_tasks = num_tasks; }
  void set_task_set(map<int, AmcTask> task_set) { this->task_set = task_set; }

  double get_utilization() const { return utilization; }
  double get_hi_probability() const { return hi_probability; }
  int get_num_tasks() const { return num_tasks; }
  int get_lo_count() const { return this->lo_tasks_list.size(); }
  int get_hi_count() const { return this->hi_tasks_list.size(); }
  map<int, AmcTask> get_task_set() const { return task_set; }

private:
  vector<AmcTask> lo_tasks_list;
  vector<AmcTask> hi_tasks_list;
  double target_u;
  double hi_probability;
  int wcet_ratio;


};

#endif
