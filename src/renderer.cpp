#include "renderer.h"
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
void renderSystemStats(double cpuUsage ,double memUsage){
    mvprintw(1,2, "System Monitor");
    mvprintw(3,2, "CPU Usage : %.2f%%" , cpuUsage*100);
    mvprintw(4,2, "Memory Usage : %.2f%%" , memUsage*100);
}