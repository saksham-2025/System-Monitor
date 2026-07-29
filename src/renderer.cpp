#include "renderer.h"
#include "system.h"
#include<algorithm>
#include<ncurses.h>

using namespace std ;
const int VISIBLE_ROWS = 10 ;
void renderProcessTable(const vector<Process> &processes,int limit ,int startIndex){
        mvprintw(10,1,"---------------------------------------------------------------------------");
        attron(COLOR_PAIR(6) | A_BOLD);
        mvprintw(11,2,"No.");
        mvprintw(11,10,"PID");
        mvprintw(11,20 ,"NAME") ;
        mvprintw(11,50,"CPU%%");
        mvprintw(11,63 , "MEMORY(KB)");
        attroff(COLOR_PAIR(6)| A_BOLD);
        mvprintw (12,1,"---------------------------------------------------------------------------");
    
        for (int idx= 0 ; idx<min(limit , int(processes.size()) - startIndex)  ; idx++){
		const Process &p = processes[startIndex + idx];
        mvprintw(idx+13 ,2 , "%d" , idx+startIndex+1);
        mvprintw(idx+13 ,10 , "%s" , p.PID.c_str());
        mvprintw(idx+13 , 20 , "%s",p.Name.c_str());
        mvprintw(idx+13 , 50, "%.2f%%" , p.cpuUsage*100);
        mvprintw(idx+13, 63 , "%ld" , p.memory);
        int size =static_cast<int>(processes.size());
        int last = min((startIndex+VISIBLE_ROWS),size);
        mvprintw(25,2,"%s%d%s%d%s%d%s","Showing ",startIndex+1 ,"-", last," of ", size , " processes");
    } 
}
void renderSystemStats(double cpuUsage ,double memUsage , const DiskStats &  diskData){
    mvprintw(5,1, "+-----------------------+");
    mvprintw(8,1 ,"+-----------------------+");
    mvprintw(6,1, "|");
    mvprintw(6,25,"|");
    attron(COLOR_PAIR(2));
    mvprintw(6,3, "CPU Usage    :");
    mvprintw(7,3, "Memory Usage :");
    attroff(COLOR_PAIR(2));
    mvprintw(6,18 , "%.2f%%" ,cpuUsage*100);
    mvprintw(7,18 , "%.2f%%" ,memUsage*100);
    mvprintw(7,1, "|");
    mvprintw(7,25, "|");
    mvprintw(6,27, "|");
    mvprintw(6,51, "|");
    mvprintw(7,27, "|");
    mvprintw(7,51, "|");
    attron(COLOR_PAIR(4));
    mvprintw(6,28,"Disk Usage   :");
    mvprintw(7,28,"Total Storage:");
    mvprintw(8,28,"Free Storage :");
    attroff(COLOR_PAIR(4));
    mvprintw(6 , 43,  "%.2f%%" , diskData.usage*100);
    mvprintw(7,43 ,"%.2f GB" , bytesToGB(diskData.totalSpace));
    mvprintw(8,43 ,"%.2f GB" , bytesToGB(diskData.freeSpace));
    mvprintw(8,27, "|");
    mvprintw(8,51, "|");
    mvprintw(5,27,"+-----------------------+");
    mvprintw(9,27,"+-----------------------+");
    
}
void renderNetworkSpeed( const pair<double,double> & netSpeed){
    mvprintw(5,53,"+---------------------+");
    attron(COLOR_PAIR(7));
    mvprintw(6,55 ,"Download :");
    mvprintw(7,55 ,"Upload   :");
    attroff(COLOR_PAIR(7));
    mvprintw(6,65 ,"%.2f Kb/s" , netSpeed.first);
    mvprintw(7,65 ,"%.2f Kb/s" , netSpeed.second);
    mvprintw(6,53, "|");
    mvprintw(6,75, "|");
    mvprintw(7,53, "|");
    mvprintw(7,75, "|");
    mvprintw(8,53,"+---------------------+");
    
}

void renderSortMessage(vector<Process> &processes , bool &sortByCPU){
    if(sortByCPU){
        sort(processes.begin(), processes.end(), compareByCPU);
        mvprintw(24 ,2 ,"Sort Mode : CPU");
    }
    else{
    sort(processes.begin(), processes.end(), compareByMemory);
    mvprintw(24,2 ,"Sort Mode : Memory");
    }
}

void renderDashboard(SystemData &data , bool sortByCPU, int startIndex){
    clear();
    mvprintw (0,1,"===========================================================================");
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw (1,25, " Next-Gen System Monitor");
    attroff(COLOR_PAIR(1) | A_BOLD);
    mvprintw (2,1,"===========================================================================");
    mvprintw (3, 15," Q Quit | C CPU | M Memory | K Kill | Up/Down Scroll ");
    mvprintw (23,1, "---------------------------------------------------------------------------" );
    renderSortMessage(data.processes,sortByCPU);
    renderSystemStats(data.cpuUsage ,data.memUsage ,data.diskData);
    renderProcessTable(data.processes,VISIBLE_ROWS , startIndex );
    renderNetworkSpeed(data.netSpeed);
    refresh() ;
}