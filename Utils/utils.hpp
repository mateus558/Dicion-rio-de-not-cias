#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED
#pragma once
#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0
void process_mem_usage(double& vm_usage, double& resident_set);
/// clear the console
void clear();
/// wait for the user to press a key
void waitUserAction();

#endif