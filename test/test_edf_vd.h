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

}




#endif