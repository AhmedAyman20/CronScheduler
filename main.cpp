#include<bits/stdc++.h>
#include "Functions/functions.h"
#include "Scheduler/scheduler.h"
using namespace std;
using namespace Scheduler;
using namespace Logger;

int main() {
    Log("🎉 Welcome to My Cron-scheduler App! Let the celebration begin! 🚀", LogLevel::INFO);
    Log("Jobs will be added to the Scheduler soon ", LogLevel::INFO);
    //This loop iterates over each job and appends it to a list
    for (const auto &job: jobs) {
        Scheduler::addJob(job.name, job.function, job.frequency, job.expectedInterval);
    }
    // We initiate a thread whose sole function is to verify if there's a job to be executed at the given time
    thread CheckingComingJobs( Scheduler::CheckJobs);
    // Writing 'exit' is the sole method to terminate the program
    string Quit;
    while (cin >> Quit) {
        if (Quit == "q") {
            Log("Thank you for Using my Cron scheduler, ", LogLevel::INFO);
            exit(0);
        }
    }
    CheckingComingJobs.join();

    return 0;
}