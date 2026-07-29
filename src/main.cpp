#include "renderer.h"  
#include "system.h" 
#include "monitor.h" 
#include<thread>
#include<mutex>
#include<atomic>
#include<ncurses.h>

using namespace std ;
mutex dataMutex ;
atomic<bool> running (true);

int main(){
initializeNcurses();
int startIndex = 0 ;
bool sortByCPU= true ;
SystemData systemData;
thread collectorThread(collectSystemData, ref(systemData),ref(running),ref(dataMutex));
while (running){
    SystemData localData ; 
    {
        lock_guard<mutex> lock(dataMutex);
        localData = systemData ;
    }
    renderDashboard(localData , sortByCPU ,startIndex);
    int ch = getch();
    int processCount = localData.processes.size();
   processInput(ch ,sortByCPU , startIndex , running , processCount);
   if(!running) break ;
}
collectorThread.join();
cleanupNcurses();
 
}