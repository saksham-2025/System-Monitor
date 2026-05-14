#ifndef RENDERER_H
#define RENDERER_H
#include "process.h"

void renderProcessTable(const std::vector<Process> &processes,int limit);
void renderSystemStats(double cpuUsage ,double memUsage ,double diskUsage);
#endif 