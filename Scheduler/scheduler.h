
#ifndef CRON_SCHEDULER_SCHEDULER_H
#define CRON_SCHEDULER_SCHEDULER_H

#include "../Logger/logger.h"

#include<bits/stdc++.h>
using namespace std;
using namespace Logger;
// This namespace houses all the functions required for the Scheduler

struct job {
    string name;
    function<void()> *function;
    int schedulingFrequency;
    int executionTime;
    chrono::system_clock::time_point nextExecitonTime;
};

namespace Scheduler {
    using JobsPtr = function<void()>;
    const int numberOfThreads = thread::hardware_concurrency();
    vector<string> jobNames;
    map<string, job> jobScheduling;

    long long convert_to_seconds(string intervalString, string JobName) {
        map<char, int> timeInterval = {{'s', 1},
                                       {'m', 60},
                                       {'h', 3600},
                                       {'d', 86400},
                                       {'w', 604800}};
        char unit = intervalString[intervalString.size() - 1];
        try {
            if (timeInterval[unit] == 0) {
                throw invalid_argument(" Invalid time unit. It must be s, m, h, d, or w");
            }
            intervalString.pop_back();
            bool allDigits = all_of(intervalString.begin(), intervalString.end(), ::isdigit);
            if (!allDigits) {
                throw logic_error(JobName + " Invalid time input format");
            }
            int totalNumberOfSecond = stoi(intervalString);
            totalNumberOfSecond *= timeInterval[unit];
            if (totalNumberOfSecond < 0) throw runtime_error("the time must be a positive number");
            return totalNumberOfSecond;
        } catch (const invalid_argument &e) {
            Log(JobName + " Caught invalid argument exception: " + e.what(), LogLevel::ERROR);
            cout << "Caught invalid argument exception: " << e.what() << endl;
            return -1;
        } catch (const out_of_range &e) {
            Log(JobName + " Caught exception: This number is too large to make a scheduler for.", LogLevel::ERROR);
            cout << "Caught exception: This number is too large to make a scheduler for." << std::endl;
        } catch (const exception &e) {
            Log(JobName + " Caught exception: " + e.what(), LogLevel::ERROR);
            cout << "Caught exception: " << e.what() << endl;
        }
        return -1;
    }

    class ThreadPool {
        /*
          This class is for thread pooling, where the constructor takes the number of threads that can run concurrently
          then a lamda function is passed with a condition true,
        */
        struct jobDetails {
            string jobName;
            function<void()> jobFunction;
            int jobExecutionTime;
        };
        vector<thread> threads;
        queue<jobDetails> tasks;
        mutex mtx;
        condition_variable cv;
        bool stop;
    public:
        ThreadPool(size_t numThreads) : stop(false) {
            for (size_t i = 0; i < numThreads; ++i) {
                threads.emplace_back([this] {
                    while (true) {
                        jobDetails job;
                        {
                            // A unique_lock object 'lock' is instantiated, which automatically acquires the mutex 'mtx'.
                            unique_lock<mutex> lock(mtx);
                            // A conditional variable is used here. It takes the mutex 'mtx' and a lambda function as arguments.
                            // If the lambda function returns true, the execution of the code continues.
                            // Otherwise, the thread is put to sleep and the lock is released.
                            // This condition is re-evaluated when either notify_one or notify_all is invoked.
                            // notify_one awakens a single, arbitrarily chosen, sleeping thread.
                            // The awakened thread then attempts to re-acquire the lock and rechecks the condition.
                            // notify_all awakens all sleeping threads, which then follow the same process.
                            cv.wait(lock, [this] { return stop || !tasks.empty(); });
                            if (stop && tasks.empty()) {
                                return; // Exit the thread if the pool is stopping and no tasks are left.
                            }
                            job = tasks.front();
                            tasks.pop();
                        }
                        this->doJob(job);  // pass the job to be executed
                    }
                });
            }
        }

        ~ThreadPool() {
            {
                unique_lock<mutex> lock(mtx);
                stop = true;
            }
            cv.notify_all(); // Notify all threads to stop.

            for (thread &thread: threads) {
                thread.join(); // Wait for all threads to finish.
            }
        }

        void enqueue(function<void()> task, string taskName, int taskExecutionTime) {
            {
                unique_lock<mutex> lock(mtx);
                tasks.push({taskName, task, taskExecutionTime});
            }
            cv.notify_one(); // Notify one threads to check for the task.
        }

        // This function is designed to return the identifier of the currently executing thread.
        string getThreadId() {
            thread::id threadId = this_thread::get_id();
            stringstream ss;
            ss << threadId;
            string mythread = ss.str();
            return mythread;
        }

        // This function runs the job, providing details about its execution time and the executing thread.
        void doJob(jobDetails job) {
            function<void()> doTask = job.jobFunction;
            string threadId = this->getThreadId();
            string JobName = job.jobName;
            Log(JobName + " is being executed by thread number: " + threadId, LogLevel::INFO);
            chrono::time_point<chrono::system_clock> start, end, expectedExecutionTime;
            start = chrono::system_clock::now();
            doTask();
            end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            Log(threadId + " is done executing " + JobName + " which takes " +
                to_string(elapsed_seconds.count()) + " seconds to run", LogLevel::INFO);
        }
    };

    // We’re making a ‘poll’ that matches the number of threads to the number of cores, because each core can only handle one thread at a time
    ThreadPool pool(numberOfThreads);

    // This function receives a task and then verifies if it’s the right time to run this task.
    bool checkTime(job &job) {
        // Get the current time
        auto current_time = chrono::system_clock::now();
        // Check if the input time is before the current time
        if (job.nextExecitonTime <= current_time) {
            Log(job.name + " is being added to the queue and will be Executed soon", LogLevel::INFO);
            return true;
        }
        return false;
    }

    // This function accepts job details and saves them in ‘jobScheduling’.
    // However, if a job with the same identifier already exists, it logs a warning and then replaces the existing job.
    void addJob(string JobName, JobsPtr JobFunction, string schedulingFrequency, string executionTime) {
        bool exists = false;
        if (jobScheduling.find(JobName) != jobScheduling.end()) {
            Log(JobName + " already exists, the program will override the previous Job", LogLevel::WARNING);
            Log("If you want to run the same function at different time span, just change the Job Name",
                LogLevel::INFO);
            exists = true;
        }
        int schedulingFrequencyInSeconds = convert_to_seconds(schedulingFrequency, JobName);
        int executionTimeInSeconds = convert_to_seconds(executionTime, JobName);
        if (schedulingFrequencyInSeconds == -1) {
            Log(JobName + " will not be added to the scheduler due to invalid Scheduling Frequency Time",
                LogLevel::ERROR);
            return;
        }
        if (executionTimeInSeconds == -1) {
            Log(JobName + " will not be added to the scheduler due to invalid Execution Time", LogLevel::ERROR);
            return;
        }
        if(!exists) jobNames.push_back(JobName);

        jobScheduling[JobName].name = JobName;
        jobScheduling[JobName].function = &JobFunction;
        jobScheduling[JobName].schedulingFrequency = schedulingFrequencyInSeconds;
        jobScheduling[JobName].executionTime = executionTimeInSeconds;
        jobScheduling[JobName].nextExecitonTime = chrono::system_clock::now();;
        Log(JobName + " is added Successfully which is going to run every " +
            to_string(schedulingFrequencyInSeconds / 60) +
            " minute",
            LogLevel::INFO);
    }


    // This function operates every 10 seconds, looking for any tasks that need to be added to the queue.
    // We’re checking tasks at the same time because each task doesn’t depend on the others.
    void CheckJobs() {
        while(true) {
            Log("Checking if there is a current jobs need to be executing ", LogLevel::INFO);
            #pragma omp parallel for
            for (int i = 0; i < jobNames.size(); ++i) {
                job job = jobScheduling[jobNames[i]];
                if (checkTime(job)) {
                    pool.enqueue(*job.function, job.name, job.executionTime);
                }
            }
            this_thread::sleep_for(10s);
        }
    }

}


#endif //CRON_SCHEDULER_SCHEDULER_H
