#ifndef SYSTEM_H
#define SYSTEM_H

#include<utility>
struct DiskStats{
    double usage ;
    long long freeSpace ;
    long long totalSpace ;
};
std::pair<long,long> getCPUData() ;
std::pair<long,long> getMemData() ;
std::pair<long,long> getNetworkData();
std::pair<double,double> calculateNetSpeed(std::pair<long,long> data1 , std::pair<long,long> data2);
double getCpuUsage(std::pair<long,long> data1 , std::pair<long,long> data2);
double getMemUsage();
double bytesToGB(long long data) ;
DiskStats getDiskData();

#endif
