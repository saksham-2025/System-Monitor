#ifndef RENDERER_H
#define RENDERER_H
#include "process.h"
#include "system.h"
void renderProcessTable(const std::vector<Process> &processes,int limit);
void renderSystemStats(double cpuUsage ,double memUsage ,DiskStats diskUsage);
void renderNetworkSpeed(std::pair<double,double> netSpeed);

#endif 