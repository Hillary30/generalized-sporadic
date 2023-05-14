import pytest
import random as r

import dbfs as dbf
import generate_tasks as gt

@pytest.fixture
def upper(): return 100

@pytest.fixture
def t(): return 1000

@pytest.fixture
def get_seed(): return 70

@pytest.fixture
def task_set(get_seed):
  r.seed(get_seed)

  return gt.Task_Set(0.8)

@pytest.fixture
def LO_task(task_set):
  return task_set.lo_tasks_list[0]

@pytest.fixture
def HI_task(task_set):
  return task_set.hi_tasks_list[0]

@pytest.fixture
def random_target_u(): return r.choice([0.5, 0.6, 0.7, 0.8, 0.9])

@pytest.fixture
def random_task(random_target_u): 
  return gt.generate_task(random_target_u)

@pytest.fixture
def random_LO_task(random_target_u):
  while True:
    random_LO_task = gt.generate_task(random_target_u)
    if random_LO_task.L == gt.Level.LO: break
  
  return random_LO_task

@pytest.fixture
def random_HI_task(random_target_u):
  while True:
    random_HI_task = gt.generate_task(random_target_u)
    if random_HI_task.L == gt.Level.HI: break
  
  return random_HI_task






''' TEST MAX_REQUESTS FUNCTION '''

def test_max_requests_is_positive_upper_greater_lower(upper):
  ''' upper > lower --> k > 0 '''

  assert dbf.max_requests(1, upper, upper - 1) > 0

def test_max_requests_is_1_upper_is_lower(upper):
  ''' upper == lower --> k == 1 '''

  assert dbf.max_requests(1, upper, upper) == 1

def test_max_requests_is_zero_upper_less_lower(upper):
  ''' upper < lower --> k == 0 '''

  assert dbf.max_requests(1, upper, upper + 1) == 0

def test_max_requests_is_n_plus_1_upper_greater_equal_lower(t):
  ''' upper >= lower --> k == n + 1 '''

  upper = r.randint(0, t * t)
  lower = r.randint(0, upper)
  T = r.randint(0, t)

  n = (upper - lower) // T

  assert dbf.max_requests(T, upper, lower) == n + 1

def test_max_requests_at_nT_and_nT_minus_one_different(t):
  ''' b - a >= T --> k_i != k_j '''

  upper = r.randint(0, t * t)
  lower = r.randint(0, upper)
  T = r.randint(0, t)
  n = (upper - lower) // T

  lower1 = upper - n * T + 1
  lower2 = upper - n * T

  assert dbf.max_requests(T, upper, lower1) != dbf.max_requests(T, upper, lower2) 

def test_max_requests_at_nT_and_n1T_minus_one_same(t):
  ''' b - a < T --> k_i == k_j '''

  upper = r.randint(0, t * t)
  lower = r.randint(0, upper)
  T = r.randint(0, t)
  n = (upper - lower) // T

  lower1 = upper - n * T
  lower2 = upper - (n + 1) * T + 1

  assert dbf.max_requests(T, upper, lower1) == dbf.max_requests(T, upper, lower2)






''' TEST LOWER FUNCTION '''

def test_lower_is_869(HI_task, t):
  assert dbf.lower(HI_task, t) == 869

def test_lower_is_positive_ts_is_D(random_task):
  ''' ts >= D --> lower > 0 '''

  assert dbf.lower(random_task, random_task.tight_D) > 0

def test_lower_is_zero_ts_less_D(random_task):
  ''' ts < D --> lower == 0 '''

  assert dbf.lower(random_task, random_task.tight_D - 1) == 0






''' TEST UPPER_CO FUNCTION '''

def test_upper_CO_is_425(HI_task, t):
  assert dbf.upper_CO(HI_task, t, t // 2) == 425
 
def test_upper_CO_less_ts(random_HI_task, t):
  ''' ∀ 0 < ts < t, upper_co < ts '''

  t = r.randint(0, t * t)

  assert all([dbf.upper_CO(random_HI_task, t, ts) < ts for ts in range(1, t)])






''' TEST UPPER_UN FUNCTION '''

def test_upper_UN_is_500(LO_task, t):
  assert dbf.upper_UN(LO_task, t, t // 2) == 500

def test_upper_UN_is_positive_t_greater_D(random_LO_task):
  ''' t > D --> upper_un > 0 '''

  assert dbf.upper_UN(random_LO_task, random_LO_task.tight_D + 1, 1) > 0






''' TEST DBF FUNCTION '''

def test_dbf_is_35_upper_greater_lower(upper):
  ''' upper > lower --> dbf == C * k '''

  assert dbf.demand_based_function(5, 15, upper, 0) == 35

def test_dbf_is_5_upper_is_lower(upper):
  ''' upper == lower --> dbf == C '''

  assert dbf.demand_based_function(5, 15, upper, upper) == 5

def test_dbf_is_zero_upper_less_lower(upper):
  ''' upper < lower --> dbf == 0 '''

  assert dbf.demand_based_function(5, 15, upper, upper + 1) == 0






''' TEST DBF_LO FUNCTION '''

def test_dbf_LO_is_70(LO_task, t):
  assert dbf.demand_based_function_LO(LO_task, t) == 70

def test_dbf_LO_is_zero_ts_less_D(random_task):
  ''' ts < tight_D --> dbf_LO == 0 '''

  assert dbf.demand_based_function_LO(random_task, random_task.tight_D - 1) == 0






''' TEST DBF_HI FUNCTION '''

def test_dbf_HI_is_300(HI_task, t):
  assert dbf.demand_based_function_HI(HI_task, t, t // 2) == 300

def test_dbf_HI_is_zero_t_ts_less_D(random_HI_task, t):
  ''' t - ts - D < 0 --> dbf_HI == 0 '''

  assert dbf.demand_based_function_HI(random_HI_task, t, t - random_HI_task.D + 1) == 0






''' TEST DBF_CO FUNCTION '''

def test_dbf_CO_is_60(HI_task, t):
  assert dbf.demand_based_function_CO(HI_task, t, t // 2) == 60

def test_dbf_CO_is_zero_t_less_D(random_HI_task):
  ''' t < D --> upper < 0 --> dbf_CO == 0 '''

  t = random_HI_task.D - 1
  ts = r.randint(1, t - 1)

  assert dbf.demand_based_function_CO(random_HI_task, t, ts) == 0

def test_dbf_CO_is_zero_ts_less_condition(random_HI_task):
  ''' t > D and ts <= t - D - (t-D)//T * T --> upper < 0 --> dbf_CO == 0 '''

  t = random_HI_task.D + 100
  ts = t - random_HI_task.D - (t - random_HI_task.D)//random_HI_task.T * random_HI_task.T

  assert dbf.demand_based_function_CO(random_HI_task, t, ts) == 0

def test_dbf_CO_is_zero_t_is_D_ts_is_Ds(random_HI_task):
  ''' t == D and ts >= tight_D --> upper == 0 and lower > 0 --> dbf_CO == 0 '''

  t = random_HI_task.D
  random_HI_task.tight_D = r.choice([random_HI_task.D // 2, random_HI_task.D])
  ts = random_HI_task.tight_D

  assert dbf.demand_based_function_CO(random_HI_task, t, ts) == 0

def test_dbf_CO_is_zero_up_less_low_for_ts(random_HI_task):
  ''' t > D and ts >= (t-D)//T * T + tight_D --> upper < lower --> dbf_CO == 0 '''

  t = random_HI_task.D + 100
  random_HI_task.tight_D = r.choice([random_HI_task.D // 2, random_HI_task.D])
  ts = (t-random_HI_task.D)//random_HI_task.T * random_HI_task.T + random_HI_task.tight_D

  assert dbf.demand_based_function_CO(random_HI_task, t, ts) == 0

def test_dbf_CO_is_zero_up_less_low_for_all(random_HI_task):
  ''' t - D - (t-D)//T * T >= tight_D --> upper < lower --> dbf_CO == 0 '''
  # need to prove, unfinished test
  t = random_HI_task.D + 100
  random_HI_task.tight_D = t - random_HI_task.D - (t-random_HI_task.D)//random_HI_task.T * random_HI_task.T
  ts = r.randint(1, t)

  assert dbf.demand_based_function_CO(random_HI_task, t, ts) == 0






''' TEST DBF_UN FUNCTION '''

def test_dbf_UN_is_28(LO_task, t):
  assert dbf.demand_based_function_UN(LO_task, t, t // 2) == 28

def test_dbf_UN_is_positive_upper_is_positive_lower_is_zero(random_LO_task):
  ''' upper > 0 and lower == 0 --> dbf_UN > 0 '''

  assert dbf.demand_based_function_UN(random_LO_task, random_LO_task.D + 1, random_LO_task.tight_D - 1) > 0

def test_dbf_UN_is_C_LO_upper_lower_is_zero_C_LO_D_min_difference_1(random_LO_task):
  ''' upper == 0 and lower == 0 --> dbf_UN == C_LO '''

  assert dbf.demand_based_function_UN(random_LO_task, random_LO_task.D, random_LO_task.tight_D - 1) == random_LO_task.C_LO

def test_dbf_UN_is_zero_upper_is_negative(random_LO_task):
  ''' upper < 0 --> dbf_UN == 0 '''

  assert dbf.demand_based_function_UN(random_LO_task, random_LO_task.D - 1, 1) == 0

def test_dbf_UN_is_zero_upper_is_zero_lower_is_positive(random_LO_task):
  ''' upper == 0 and lower > 0 --> dbf_UN == C_LO '''

  assert dbf.demand_based_function_UN(random_LO_task, random_LO_task.D, random_LO_task.tight_D) == 0






''' TEST SUM_DBF FUNCTION '''

def test_sum_dbf_LO_is_289(task_set, t):
  assert dbf.sum_dbf_LO(task_set, t // 2) == 289

def test_sum_dbf_HI_is_634(task_set, t):
  assert dbf.sum_dbf_HI(task_set, t, t // 2) == 634

def test_sum_dbf_CO_is_226(task_set, t):
  assert dbf.sum_dbf_CO(task_set, t, t // 2) == 226

def test_sum_dbf_UN_is_62(task_set, t):
  assert dbf.sum_dbf_UN(task_set, t, t // 2) == 62

def test_sum_dbf_is_1211(task_set, t):
  assert dbf.sum_dbf(task_set, t, t // 2) == 289 + 634 + 226 + 62