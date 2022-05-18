import pytest
import dbfs as dbf
import generate_tasks as gt
import random as r

def test_demand_based_function():
  assert dbf.demand_based_function(7, 4, 6, 4) == 4

def test_demand_based_function_always_nonnegative():
  T = gt.generate_T()
  C = gt.generate_C_Lo(T)
  D = gt.generate_D(C)

  assert dbf.demand_based_function(r.randrange(5, 101), C, D, T) >= 0

def test_is_carry_over_exists_returns_true():
  assert dbf.is_carry_over(7, 11, 4, 4)

def test_is_carry_over_exists_returns_false_for_first_condition():
  assert dbf.is_carry_over(7, 11, 2, 4) is False

def test_is_carry_over_exists_returns_false_for_second_condition():
  assert dbf.is_carry_over(6, 8, 5, 4) is False

def test_dbf_CO_returns_CHi():
  assert dbf.demand_based_function_CO(7, 1, 2, 4, gt.Level.HI) == 2

def test_dbf_CO_returns_CLo():
  assert dbf.demand_based_function_CO(7, 1, 1, 4, gt.Level.LO) == 1

def test_dbf_CO_returns_ts_mod_T():
  assert dbf.demand_based_function_CO(7, 4, 4, 4, gt.Level.LO) == 3

def test_lcm():
  assert dbf.lcm([4,5,12,15,30]) == 60

def test_calculate_t_max_returns_60():
  r.seed(36)
  
  assert dbf.calculate_t_max(gt.create_task_set()) == 160

def test_calculate_t_max_always_returns_nonnegative():
  assert dbf.calculate_t_max(gt.create_task_set()) > 0

def test_calculate_dbf_always_nonnegative():
  t = r.randint(2, 100)
  assert dbf.calculate_dbf(t, r.randint(1,t - 1), gt.create_task_set()) >= 0

def test_calculate_dbf_with_80_seed():
  r.seed(80)
  t = r.randint(100, 150)
  ts = t//2
  task_set = gt.create_task_set()

  assert dbf.calculate_dbf(t, ts, task_set) == 61

def test_calculate_dbf_with_15_seed():
  r.seed(15)
  t = r.randint(100, 150)
  ts = t//2
  task_set = gt.create_task_set()

  assert dbf.calculate_dbf(t, ts, task_set) == 48

def test_schedulability_test_80_seed_returns_true():
  r.seed(80)

  assert dbf.schedulability_test(gt.create_task_set())

def test_schedulability_test_21_seed_returns_false():
  r.seed(21)

  assert dbf.schedulability_test(gt.create_task_set()) is False

