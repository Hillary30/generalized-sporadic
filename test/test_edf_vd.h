#ifndef EDF_VD_TEST
#define EDF_VD_TEST

#include <gtest/gtest.h>
#include "../src/edf_vd.h"

TEST(EDF_VD, EDFDemandBasedFunction) {
    Task task1 = Task(0, 5, 2, 2, 3, 3);
    Task task2 = Task(1, 6, 2, 3, 4, 4);
    int t = 10;

    EXPECT_EQ(edf_vd_demand_based_function(task1.C_LO, task1.T, t, 0), 4);
    EXPECT_EQ(edf_vd_demand_based_function(task2.C_LO, task2.T, t, 0), 2);
    EXPECT_EQ(edf_vd_demand_based_function(task2.C_HI, task2.T, t, 0), 3);

}

TEST(EDF_VD, SumLoadFunction) {
    map<string, vector<Task>> task_set_dict = {};
    Task task1 = Task(0, 5, 2, 2, 3, 3);
    Task task2 = Task(1, 6, 2, 3, 4, 4);
    task_set_dict["lo"] = {task1};
    task_set_dict["hi"] = {task2};
    TaskSet task_set = TaskSet(task_set_dict);

    int t = 10;
    EXPECT_EQ(sum_load(task_set, t, 0), 0.7);
    EXPECT_EQ(sum_load_LO(task_set, t, 0), 0.6);
    EXPECT_EQ(sum_load_HI(task_set, t, 0), 0.3);
    // EXPECT_EQ(sum_load(task_set, t, 0), 7);
    // EXPECT_EQ(sum_load_LO(task_set, t, 0), 6);
    // EXPECT_EQ(sum_load_HI(task_set, t, 0), 3);

}

TEST(EDF_VD, SchedulabilityLemma44) {
    map<string, vector<Task>> task_set_dict = {};
    Task task1 = Task(0, 5, 2, 2, 3, 3);
    Task task2 = Task(1, 6, 2, 3, 4, 4);
    task_set_dict["lo"] = {task1};
    task_set_dict["hi"] = {task2};
    TaskSet task_set = TaskSet(task_set_dict);

    int t = 10;
    EXPECT_EQ(schedulability_lemma_44(task_set, t, 0), true);
    // EXPECT_EQ(offline_pp(task_set, t, 0), true);
    // EXPECT_EQ(task_set.opp_klevel, 2);
    
    map<string, vector<Task>> task_set_dict2 = {};
            //   Task(I, T, LO, HI, D, DT)
    Task task3 = Task(0, 10, 2, 3, 4, 4);
    Task task4 = Task(1, 10, 2, 4, 4, 4);
    Task task5 = Task(2, 10, 2, 4, 4, 4);
    task_set_dict2["hi"] = {task3, task4, task5};
    TaskSet task_set2 = TaskSet(task_set_dict2);

    EXPECT_EQ(schedulability_lemma_44(task_set2, t, 0), false);
    // EXPECT_EQ(offline_pp(task_set2, t, 0), false);
    // EXPECT_EQ(task_set2.opp_klevel, -1);
}

TEST(EDF_VD, SchedulabilityLemma46) {
    //true == unschedulable
    map<string, vector<Task>> task_set_dict = {};
            //   Task(I, T, LO, HI, D, DT)
    Task task1 = Task(0, 10, 2, 3, 4, 4); //load_lo = 5, load_hi = 11
    Task task2 = Task(1, 10, 2, 4, 4, 4);
    Task task3 = Task(2, 10, 2, 4, 4, 4);
    task_set_dict["hi"] = {task1, task2, task3};
    TaskSet task_set = TaskSet(task_set_dict);

    int t = 10;
    cout << "sum_load: " << sum_load(task_set, t, 0) << endl;
    cout << "sum_load_lo: " << sum_load_LO(task_set, t, 0) << endl;
    cout << "sum_load_hi: " << sum_load_HI(task_set, t, 0) << endl;
    EXPECT_EQ(schedulability_lemma_46(sum_load_LO(task_set, t, 0), sum_load_HI(task_set, t, 0)), false);
    // EXPECT_EQ(offline_pp(task_set, t, 0), false);
    // EXPECT_EQ(task_set.opp_klevel, -1);

    map<string, vector<Task>> task_set_dict2 = {};
            //   Task(I, T, LO, HI, D, DT)
    Task task4 = Task(0, 10, 7, 7, 3, 3); //lo = 8
    Task task5 = Task(1, 10, 1, 2, 4, 4); //hi = 2
    task_set_dict2["lo"] = {task4};
    task_set_dict2["hi"] = {task5};
    TaskSet task_set2 = TaskSet(task_set_dict2);
    EXPECT_EQ(schedulability_lemma_46(sum_load_LO(task_set2, t, 0), sum_load_HI(task_set2, t, 0)), true);


}


#endif