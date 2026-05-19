#include "renderer.h"
#include "system.h"
#include <iomanip>
#include<iostream>
#include<algorithm>
#include<ncurses.h>

using namespace std ;

void renderProcessTable(const vector<Process> &processes,int limit){
        mvprintw(6,2,"PID");
        mvprintw(6,10 ,"NAME") ;
        mvprintw(6,50,"CPU%%");
        mvprintw(6,60 , "MEMORY(KB)");
        mvprintw (7,0,"-------------------------------------------------------------------------");
    
        for (int idx= 0 ; idx<min(limit , int(processes.size()))  ; idx++){
		const Process &p = processes[idx];
        mvprintw(idx+9 ,2 , "%s" , p.PID.c_str());
        mvprintw(idx+9 , 10 , "%s",p.Name.c_str());
        mvprintw(idx+9 , 50, "%.2f%%" , p.cpuUsage*100);
        mvprintw(idx+9 , 60 , "%ld" , p.memory);
    } 
}
void renderSystemStats(double cpuUsage ,double memUsage , DiskStats diskData){
    mvprintw(1,2, "System Monitor");
    mvprintw(2,2, "CPU Usage    : %.2f%%" , cpuUsage*100);
    mvprintw(3,2, "Memory Usage : %.2f%%" , memUsage*100);
    mvprintw(2,50,"Disk Usage   : %.2f%%" , diskData.usage*100);
    mvprintw(3,50,"Total Storage: %.2f GB" , bytesToGB(diskData.totalSpace) );
    mvprintw(4,50,"Free Storage : %.2f GB" , bytesToGB(diskData.freeSpace) );

}
void renderNetworkSpeed(pair<double,double> netSpeed){
    mvprintw(20 , 40 ,"Download: %.2f Kb/sec" , netSpeed.first);
    mvprintw(21 , 40 ,"Upload  : %.2f Kb/sec" , netSpeed.second);

}