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
keypad(stdscr, TRUE);
timeout(100);
bool sortByCPU= true ;
while (true){
    clear();
    mvprintw(0, 2,"Press q to quit | c = CPU sort | m = Memory sort");
    auto data1 = getCPUData();
    vector<Process> processes = takeProcessSnapshot();
    sleep(1);
    auto data2 = getCPUData();
    calculateProcessCpuUsage(processes , data2.first - data1.first);
    double cpuUsage = getCpuUsage(data1 ,data2);
    double memUsage=  getMemUsage();
    // sort(processes.begin(),processes.end(),compareByCPU);
    if(sortByCPU)
{
    sort(processes.begin(), processes.end(), compareByCPU);
    mvprintw(22 ,2 ,"SORT BY CPU");
}
else
{
    sort(processes.begin(), processes.end(), compareBYMemory);
    mvprintw(22 ,2 ,"SORT BY MEMORY");
    
}
    renderSystemStats(cpuUsage ,memUsage);
    renderProcessTable(processes,10);
    refresh() ;
    int ch = getch();
    if(ch == 'q') break ;
    else if(ch=='c') {
        sortByCPU =true ;
    }
    else if(ch=='m'){
        sortByCPU = false ;
    }    
}
endwin();
 
}