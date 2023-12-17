#include "functions.h"
#include<bits/stdc++.h>
using namespace std;

// This is the location where you should implement your C++ functions, which have already been declared in the header file.
// The function logic and implementation should be will tested separately before give to the scheduler

int x = 1;

void greet(){
    cout << "Thread: " << this_thread::get_id() << " is Executing The Greet Function x " << x  << endl;
    x++;
    for (int i = 0; i < 2e9; ++i) {
        int y = i;
    }
    cout << "Thread: " << this_thread::get_id() << " is Done Executing The Greet Function"  << endl;
}

void sub(){
    cout << "Thread: " << this_thread::get_id() << " is Executing The Sub Function"  << endl;

    for (int i = 0; i < 2e9; ++i) {
        int x = i;
    }
    cout << "Thread: " << this_thread::get_id() << " is Done Executing The Sub Function"  << endl;
}

void multiply(){
    cout << "Thread: " << this_thread::get_id() << " is Executing The Multiply Function"  << endl;
    for (int i = 0; i < 2e9; ++i) {
        int x = i;
    }
    cout << "Thread: " << this_thread::get_id() << " is Done Executing The Multiply Function"  << endl;
}


// The Scheduler reads from the Jobs Vector, where each field should include
// {Job Name, function reference, scheduling frequency, expected interval}

vector<Job> jobs = {
        {"greet", &greet, "10s", "1s"},
        {"sub", &sub, "1m", "2m"},
        {"multiply", &multiply, "3h", "3s"},
        {"greet1", &greet, "1111111111111d", "1s"},
        {"greet2", &greet, "1111111111111sd", "1s"},
        {"greet2", &greet, "5s", "1s"},


};
