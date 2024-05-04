#ifndef AMC_TEST
#define AMC_TEST

#include <gtest/gtest.h>

#include "../src/amc.h"

TEST(AMC, BusyPeriodLO) {
    map<string, vector<Task>> task_set_dict = {};
    task_set_dict["lo"] = {Task(0, 10, 2, 2, 12, -1, LO, -1)};
    task_set_dict["hi"] = {Task(1, 20, 4, 5, 25, -1, HI, -1), \
                           Task(2, 40, 6, 7, 60, -1, HI, -1)};
    TaskSet task_set = TaskSet(task_set_dict);

    // Test task 3 lowest priority
    task_set.get_task_set_ref().at(2).priority = 3;
    EXPECT_EQ(compute_busy_period_lo (0, task_set, 2), 14);

    // Test task 2 and 3 lowest priorities
    task_set.get_task_set_ref().at(1).priority = 2;
    EXPECT_EQ(compute_busy_period_lo (0, task_set, 1), 6);

    // Test task 1 highest priority
    EXPECT_EQ(compute_busy_period_lo (0, task_set, 0), 2);
}

TEST(AMC, ResponseTimeLO) {
    map<string, vector<Task>> task_set_dict = {};
    task_set_dict["lo"] = {Task(0, 10, 2, 2, 12, -1, LO, -1)};
    task_set_dict["hi"] = {Task(1, 20, 4, 5, 25, -1, HI, -1), \
                           Task(2, 10, 6, 7, 60, -1, HI, -1)};
    TaskSet task_set = TaskSet(task_set_dict);
    int lastRelease;

    // Test task 3 lowest priority
    task_set.get_task_set_ref().at(2).priority = 3;
    EXPECT_EQ(compute_response_time_lo (task_set, 2, lastRelease), 14);

    // Test task 2 and 3 lowest priorities
    task_set.get_task_set_ref().at(1).priority = 2;
    EXPECT_EQ(compute_response_time_lo (task_set, 1, lastRelease), 6);

    // Test task 1 highest priority
    EXPECT_EQ(compute_response_time_lo (task_set, 0, lastRelease), 2);
}

TEST(AMC, BusyPeriodHI) {
    map<string, vector<Task>> task_set_dict = {};
    task_set_dict["lo"] = {Task(0, 10, 2, 2, 12, -1, LO, -1)};
    task_set_dict["hi"] = {Task(1, 20, 4, 5, 25, -1, HI, -1), \
                           Task(2, 10, 6, 7, 60, -1, HI, -1)};
    TaskSet task_set = TaskSet(task_set_dict);
    
    // Test task 3 lowest priority, min(q,p) = 0
    task_set.get_task_set_ref().at(2).priority = 3;
    EXPECT_EQ(compute_busy_period_hi(0, task_set, 2, 1), 16);

    // Test task 3 lowest priority, min(q,p) = 1
    EXPECT_EQ(compute_busy_period_hi(1, task_set, 2, 1), 28);

    // Test tasks 2 and 3 lowest priority, min(q,p) = 0
    task_set.get_task_set_ref().at(1).priority = 2;
    EXPECT_EQ(compute_busy_period_hi(0, task_set, 1, 0), 7);

    // Test task 1 highest priority
    EXPECT_EQ(compute_busy_period_hi(0, task_set, 0, 0), 2);
}

TEST(AMC, ResponseTimeHI) {
    map<string, vector<Task>> task_set_dict = {};
    task_set_dict["lo"] = {Task(0, 10, 2, 2, 12, -1, LO, -1)};
    task_set_dict["hi"] = {Task(1, 20, 4, 5, 25, -1, HI, -1), \
                           Task(2, 10, 6, 7, 60, -1, HI, -1)};
    TaskSet task_set = TaskSet(task_set_dict);
    
    // Test task 3 lowest priority, last released task p = 1
    task_set.get_task_set_ref().at(2).priority = 3;
    EXPECT_EQ(compute_response_time_hi(task_set, 2, 1), 18);

    //  Test tasks 2 and 3 lowest priority, min(q,p) = 0
    task_set.get_task_set_ref().at(1).priority = 2;
    EXPECT_EQ(compute_response_time_hi(task_set, 1, 0), 7);

    // Test task 1 highest priority
    EXPECT_EQ(compute_response_time_hi(task_set, 0, 0), 2);
}

TEST(AMC, AMCSchedulability) {
    map<string, vector<Task>> task_set_dict = {};
    task_set_dict["lo"] = {Task(0, 10, 2, 2, 12, -1, LO, -1)};
    task_set_dict["hi"] = {Task(1, 20, 4, 5, 25, -1, HI, -1), \
                           Task(2, 10, 6, 7, 60, -1, HI, -1)};
    TaskSet task_set = TaskSet(task_set_dict);

    // Test schedulability with task 3 lowest priority
    task_set.get_task_set_ref().at(2).priority = 3;
    EXPECT_TRUE(amc_schedulability_test(task_set, 2));

    // Test schedulability with tasks 2 and 3 lowest priority
    task_set.get_task_set_ref().at(1).priority = 2;
    EXPECT_TRUE(amc_schedulability_test(task_set, 1));

    // Test schedulabilty with task 1 highest priority
    EXPECT_TRUE(amc_schedulability_test(task_set, 0));
}

TEST(AMC, AudsleyOPA) {
    map<string, vector<Task>> task_set_dict = {};
    task_set_dict["lo"] = {Task(0, 10, 2, 2, 12, -1, LO, -1)};
    task_set_dict["hi"] = {Task(1, 20, 4, 5, 25, -1, HI, -1), \
                           Task(2, 10, 6, 7, 60, -1, HI, -1)};
    TaskSet task_set = TaskSet(task_set_dict);
    EXPECT_TRUE(audsleys_optimal_priorirty_assignment(task_set));

    task_set_dict.clear();
    task_set_dict["lo"] = {Task(0, 10, 6, 6, 12, -1, LO, -1)};
    task_set_dict["hi"] = {Task(1, 15, 8, 10, 14, -1, HI, -1), \
                           Task(2, 20, 10, 12, 18, -1, HI, -1)};
    TaskSet task_set_2 = TaskSet(task_set_dict);
    EXPECT_FALSE(audsleys_optimal_priorirty_assignment(task_set_2));
}

#endif