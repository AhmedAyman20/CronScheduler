// The header file that serves as an interface for your function.

#ifndef TASK_FUNCTIONS_H
#define TASK_FUNCTIONS_H
#include<bits/stdc++.h>

using namespace std;

// This is where you insert the function name.
void greet();

void sub();

void multiply();


// Keep this unchanged, as you will be entering your job details in the function file.

struct Job {
    string name;
    void (*function)();
    string frequency;
    string expectedInterval;
};

extern vector<Job> jobs;

#endif //TASK_FUNCTIONS_H

