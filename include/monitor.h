#ifndef MONITOR_H
#define MONITOR_H

#include "system.h"
#include<atomic>
#include<mutex>

void initializeNcurses();
void cleanupNcurses();

void handleKeyboardInput(char ch,bool& sortByCPU);

void handleKillProcess();
void processInput(int ch, bool& sortByCPU, std::atomic<bool> & running);
void collectSystemData(SystemData &systemData,std::atomic<bool> & running, std::mutex &dataMutex);

#endif