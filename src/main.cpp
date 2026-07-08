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
bool sortByCPU= true ;
SystemData systemData;
thread collectorThread(collectSystemData, ref(systemData),ref(running),ref(dataMutex));
while (running){
    SystemData localData ; 
    {
        lock_guard<mutex> lock(dataMutex);
        localData = systemData ;
    }
    renderDashboard(localData , sortByCPU);
    int ch = getch();
   processInput(ch ,sortByCPU , running);
   if(!running) break ;
}
collectorThread.join();
cleanupNcurses();
 
}