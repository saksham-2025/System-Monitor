#include "process.h"
#include "renderer.h"  
#include "system.h" 
#include<iostream> 
#include <unistd.h> 
#include<algorithm>   //For sorting
#include<iomanip>
#include<ncurses.h>
using namespace std ;
      

int main(){
initscr();
while (true){
    clear();
    auto data1 = getCPUData();
    vector<Process> processes = takeProcessSnapshot();
    sleep(1);
    auto data2 = getCPUData();
    calculateProcessCpuUsage(processes , data2.first - data1.first);
    double cpuUsage = getCpuUsage(data1 ,data2);
    double memUsage=  getMemUsage();
    renderSystemStats(cpuUsage ,memUsage);
    sort(processes.begin(),processes.end(),compareByCPU);
    renderProcessTable(processes,10);
    refresh() ;
    
}
endwin();
 
}