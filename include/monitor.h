#ifndef MONITOR_H
#define MONITOR_H

#include "system.h"
#include<atomic>
#include<mutex>

void initializeNcurses();
void cleanupNcurses();

void handleKeyboardInput(char ch,bool& sortByCPU);

void handleKillProcess();
void processInput(int ch, bool& sortByCPU, int &startIndex ,std::atomic<bool> & running, int processCount );
void collectSystemData(SystemData &systemData,std::atomic<bool> & running, std::mutex &dataMutex);

#endif