#ifndef RENDERER_H
#define RENDERER_H
#include "process.h"
#include "system.h"
void renderProcessTable(const std::vector<Process> &processes,int limit);
void renderSystemStats(double cpuUsage ,double memUsage ,DiskStats diskUsage);
void renderNetworkSpeed(std::pair<double,double> netSpeed);
void renderDashboard(SystemData &data , bool sortByCPU); 
void renderSortMessage(std::vector<Process> &processes , bool &sortByCPU);

#endif 