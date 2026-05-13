#ifndef PROCESS_H
#define PROCESS_H
#include<string> 
#include<vector>

struct Process{
    std::string PID ;
    std::string Name ; 
    long memory ;
    long oldCPUTime ;
    long newCPUTime ;
    double cpuUsage ;
};
//To check whether the file name is numeric or not
bool isNumeric(std::string name) ;
// To get the name of that process
std::string getProcessName(std::string path);

long getProcessVmRSS(std::string path) ;
long getProcessCPUTime(std::string path);
bool compareByCPU(const Process &a ,const Process &b);
bool compareByMemory(const Process &a , const Process &b);
std::vector<Process> takeProcessSnapshot();
void calculateProcessCpuUsage(std::vector<Process> &processes , double CpuDelta );

#endif