# Cron-scheduler

This Cron Scheduler project, implemented in C++. This project is designed to
automate and manage tasks efficiently and effectively.

## Requirements

- c++17 or a higher version.

## Components

This project is structured into several key components, each playing a crucial role in the functionality of the Cron
Scheduler:

- **main.cpp**: This is the entry point of the project where the execution begins.

- **Function**: This component is responsible for task management.
    - **function.h**: This file contains the interface of the tasks that will be passed to the scheduler.
    - **function.cpp**: This file contains the implementations of the functions and a vector that will hold the jobs.

- **Logger**: This component handles logging.
    - **logger.h**: This file contains a namespace logger that includes functions for logging in a file.

- **Scheduler**: This component handles the scheduling logic.
    - **scheduler.h**: This file contains a namespace scheduler that encapsulates all the logic of the Cron and core
      scheduler functions.

## How It Works

The Cron Scheduler allows users to schedule tasks by providing the following details:

- **Unique Job Identifier**: A unique identifier for each job, allowing for efficient management and tracking of tasks
- **Job Implementation**: The actual function or task that needs to be executed.
- **Scheduling Frequency**: The frequency at which the task should be executed. For instance, a task could be scheduled
  to run every hour.
- **Expected Interval**: The expected duration for a single run of the task. For example, a task might be expected to
  complete within 30 minutes.

**Termination**: To terminate the program, simply input `q`.

Once the task details are provided, the scheduler adds the tasks to a queue. When the time comes for a task to be
executed, the scheduler runs the task according to the specified frequency and expected interval.

## Usage

- **In Functions directory:**
   - `function.h`: Declare all your functions in this file of type void.
   - leave the below code as it is in the file.
   ```cpp
      struct Job {
        string name;
        void (*function)();
        string frequency;
        string expectedInterval;
      };
      extern vector<Job> jobs;
    ```
   - `functions.cpp`: Implement your functions in this file.
      - It must contain`#include "functions.h"`.
      - Then add your function to the jobs vector and this how you could add it.
     ```cpp
     vector<Job> jobs = {
        {"greet", &greet, "10s", "1s"},
        {"sub", &sub, "1m", "2m"},
    
A sample of the Functions directory has been provided for you to review its structure.

That's it! Your functions are now ready to be scheduled.

# Technical Decisions and Reasoning

1. **Utilization of Map**: A map, a sorted data structure in C++, is used due to its efficient log(n) time complexity
   for insert, update, and delete operations. This becomes particularly beneficial as the project scales and becomes
   more interactive. In this implementation, an array was supposed to be used in conjunction with a map. as there is no
   deletion in the current implementation, leveraging the O(1) access time of arrays.

2. **Parallel Execution with OpenMP**: OpenMP is employed for parallel execution of independent tasks within for loops.
   This is particularly useful when checking if the execution time for a task has arrived.

3. **Thread Pool Class Creation**: A thread pool class is created which initiates a number of threads equal to the
   number of cores. This approach is taken to avoid overhead as each core can handle only one thread at a time. These
   threads remain in a sleeping state until either a task is inserted into the queue or the program needs to be
   terminated and joined. This is managed using a Mutex and a unique_lock(), which automatically acquires the Mutex.
   conditional variable is used, taking the Mutex and a lambda function as arguments. If the lambda function returns
   true, the code execution continues and the lock will be released after finishing the scope. Otherwise, the thread is
   put to sleep and the lock is released. This condition is re-evaluated when either notify_one or notify_all is
   invoked. notify_one awakens a single, arbitrarily chosen, sleeping thread. The awakened thread then attempts to
   re-acquire the lock and rechecks the condition. notify_all awakens all sleeping threads, which then follow the same
   process.

# Potential Future Enhancements

1. **Dynamic Task Management**: Improve the system to be more dynamic, allowing users to add, remove, or modify tasks
   during runtime. This would provide users with greater control and flexibility in managing their tasks.

2. **Expected Interval Enforcement**: Implement a feature that uses the Expected Interval in a way to terminate the
   function if it exceeds the expected interval. This would ensure tasks do not overrun their allotted time, maintaining
   the efficiency and reliability of the scheduler.