#ifndef SYSTEM_H
#define SYSTEM_H

#include<utility>
#include<vector>
#include "process.h"
struct DiskStats{
    double usage = 0.0 ;
    long long freeSpace = 0  ;
    long long totalSpace = 0;
};
struct SystemData{
    double cpuUsage = 0.0 ; 
    double memUsage = 0.0 ;
    DiskStats diskData ;
    std::pair<double,double>netSpeed{0.0,0.0};
    std::vector <Process> processes;
};
std::pair<long,long> getCPUData() ;
std::pair<long,long> getMemData() ;
std::pair<long,long> getNetworkData();
std::pair<double,double> calculateNetSpeed(const std::pair<long,long> & data1 ,const std::pair<long,long> & data2);
double getCpuUsage(const std::pair<long,long> &data1 ,const std::pair<long,long> &data2);
double getMemUsage();
double bytesToGB(long long data) ;
DiskStats getDiskData();
#endif
