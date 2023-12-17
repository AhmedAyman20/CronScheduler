cmake_minimum_required(VERSION 3.26)
project(Cron_scheduler)

set(CMAKE_CXX_STANDARD 17)

include_directories(Functions)
include_directories(Logger)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(Cron_scheduler
        Functions/functions.cpp
        Functions/functions.h
        Logger/logger.h
        main.cpp
        Scheduler/scheduler.h
)
