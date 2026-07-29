#ifndef RENDERER_H
#define RENDERER_H
#include "process.h"
#include "system.h"
void renderProcessTable(const std::vector<Process> &processes,int limit ,int startIndex);
void renderSystemStats(double cpuUsage ,double memUsage ,const DiskStats & diskUsage);
void renderNetworkSpeed(const std::pair<double,double> &netSpeed);
void renderDashboard(SystemData &data , bool sortByCPU , int startIndex); 
void renderSortMessage(std::vector<Process> &processes , bool &sortByCPU);

#endif 