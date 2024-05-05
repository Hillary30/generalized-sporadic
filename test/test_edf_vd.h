#ifndef EDF_VD_TEST
#define EDF_VD_TEST

#include <gtest/gtest.h>
#include "../src/edf_vd.h"

//edf_valid_max_requests

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

}

//schedulability_prop_42
TEST(EDF_VD, SchedulabilityProp42) {
    map<string, vector<Task>> task_set_dict = {};
    Task task1 = Task(0, 5, 2, 2, 3, 3); //LO
    Task task2 = Task(1, 6, 2, 3, 4, 4); //HI
    task_set_dict["lo"] = {task1};
    task_set_dict["hi"] = {task2};
    TaskSet task_set = TaskSet(task_set_dict);

    int t = 10;
    EXPECT_EQ(schedulability_prop_42(sum_load_LO(task_set, t, 0), sum_load_HI(task_set, t, 0)), true);
    
    map<string, vector<Task>> task_set_dict2 = {};
    Task task3 = Task(0, 10, 2, 3, 4, 4); //Lo = 6
    Task task4 = Task(1, 10, 2, 4, 4, 4); //Hi = 11
    Task task5 = Task(2, 10, 2, 4, 4, 4);
    task_set_dict2["hi"] = {task3, task4, task5};
    TaskSet task_set2 = TaskSet(task_set_dict2);

    EXPECT_EQ(schedulability_prop_42(sum_load_LO(task_set2, t, 0), sum_load_HI(task_set2, t, 0)), false);
}

TEST(EDF_VD, SchedulabilityProp43) {
    map<string, vector<Task>> task_set_dict = {};
    Task task1 = Task(0, 5, 2, 2, 3, 3); //LO = 4
    Task task2 = Task(1, 6, 2, 3, 4, 4); //HI = 3
    task_set_dict["lo"] = {task1};
    task_set_dict["hi"] = {task2};
    TaskSet task_set = TaskSet(task_set_dict);

    int t = 10;
    EXPECT_EQ(schedulability_prop_43(sum_load(task_set, t, 0), sum_load_LO(task_set, t, 0), sum_load_HI(task_set, t, 0)), true);
}

//if_eligible_edf
TEST(EDF_VD, SchedulabilityIsElligibleEDF) {
        map<string, vector<Task>> task_set_dict = {};
    Task task1 = Task(0, 5, 2, 2, 3, 3); //LO = 4
    Task task2 = Task(1, 6, 2, 3, 4, 4); //HI = 3
    task_set_dict["lo"] = {task1};
    task_set_dict["hi"] = {task2};
    TaskSet task_set = TaskSet(task_set_dict);

    int t = 10;
    EXPECT_EQ(is_eligible_edf(sum_load(task_set, t, 0), sum_load_LO(task_set, t, 0), sum_load_HI(task_set, t, 0)), true);

    map<string, vector<Task>> task_set_dict2 = {};
    Task task3 = Task(0, 10, 2, 3, 4, 4);
    Task task4 = Task(1, 10, 2, 4, 4, 4);
    Task task5 = Task(2, 10, 2, 4, 4, 4);
    task_set_dict2["hi"] = {task3, task4, task5};
    TaskSet task_set2 = TaskSet(task_set_dict2);

    EXPECT_EQ(is_eligible_edf(sum_load(task_set2, t, 0), sum_load_LO(task_set2, t, 0), sum_load_HI(task_set2, t, 0)), false);

}


TEST(EDF_VD, SchedulabilityLemma44) {
    map<string, vector<Task>> task_set_dict = {};
    Task task1 = Task(0, 5, 2, 2, 3, 3);
    Task task2 = Task(1, 6, 2, 3, 4, 4);
    task_set_dict["lo"] = {task1};
    task_set_dict["hi"] = {task2};
    TaskSet task_set = TaskSet(task_set_dict);

    int t = 10;
    EXPECT_EQ(schedulability_lemma_44(sum_load(task_set, t, 0)), true);
    
    map<string, vector<Task>> task_set_dict2 = {};
    Task task3 = Task(0, 10, 2, 3, 4, 4);
    Task task4 = Task(1, 10, 2, 4, 4, 4);
    Task task5 = Task(2, 10, 2, 4, 4, 4);
    task_set_dict2["hi"] = {task3, task4, task5};
    TaskSet task_set2 = TaskSet(task_set_dict2);

    EXPECT_EQ(schedulability_lemma_44(sum_load(task_set2, t, 0)), false);

}

TEST(EDF_VD, SchedulabilityLemma46) {
    map<string, vector<Task>> task_set_dict = {};
    Task task1 = Task(0, 10, 2, 3, 4, 4); 
    Task task2 = Task(1, 10, 2, 4, 4, 4);
    Task task3 = Task(2, 10, 2, 4, 4, 4);
    task_set_dict["hi"] = {task1, task2, task3};
    TaskSet task_set = TaskSet(task_set_dict);

    int t = 10;
    EXPECT_EQ(schedulability_lemma_46(sum_load_LO(task_set, t, 0), sum_load_HI(task_set, t, 0)), false);

    map<string, vector<Task>> task_set_dict2 = {};
    Task task4 = Task(0, 10, 7, 7, 7, 7);  //load = 9
    Task task5 = Task(1, 10, 1, 2, 4, 4);  //lo = 8, hi = 2
    task_set_dict2["lo"] = {task4};
    task_set_dict2["hi"] = {task5};
    TaskSet task_set2 = TaskSet(task_set_dict2);
    EXPECT_EQ(schedulability_lemma_46(sum_load_LO(task_set2, t, 0), sum_load_HI(task_set2, t, 0)), true);

}


TEST(EDF_VD, OfflinePreProcessing) {
    map<string, vector<Task>> task_set_dict = {};
    Task task1 = Task(0, 5, 2, 2, 3, 3);
    Task task2 = Task(1, 6, 2, 3, 4, 4);
    task_set_dict["lo"] = {task1};
    task_set_dict["hi"] = {task2};
    TaskSet task_set = TaskSet(task_set_dict);

    EXPECT_EQ(offline_pp(task_set, 10, 0), true);
    EXPECT_NE(task_set.task_set[0].tight_D, -1);
}



#endif