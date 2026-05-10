#ifndef SYSTEM_H
#define SYSTEM_H

#include<utility>

std::pair<long,long> getCPUData() ;
std::pair<long,long> getMemData() ;
double getCpuUsage(std::pair<long,long> data1 , std::pair<long,long> data2);
double getMemUsage();
#endif
