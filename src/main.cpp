#include "process.h"
#include "renderer.h"  
#include "system.h" 
#include<iostream> 
#include <unistd.h> 
#include<algorithm>   //For sorting
#include<iomanip>
#include<ncurses.h>
#include<signal.h> //Linux api to send signal to process
using namespace std ;

void handleKeyboardInput(char ch ,bool &sortByCPU){
     if(ch=='c') {
        sortByCPU =true ;
    }
    else if(ch=='m'){
        sortByCPU = false ;
    } 
     
}
void renderSortMessage(vector<Process> &processes , bool &sortByCPU){
if(sortByCPU){
    sort(processes.begin(), processes.end(), compareByCPU);
    mvprintw(20 ,2 ,"SORT BY CPU");
    }
else{
    sort(processes.begin(), processes.end(), compareByMemory);
    mvprintw(20 ,2 ,"SORT BY MEMORY");
    }
}
void handleKillProcess(){
      timeout(-1);
        echo();
        mvprintw(24 , 2 , "Enter the Process Id: ") ;
        int pid ;
        scanw("%d",&pid);
        noecho();
        timeout(100);
        int result =kill(pid , SIGTERM);
        if (result ==0) mvprintw(22 ,2 , "Process Successfully Terminated ");
        else mvprintw(22,2 ,"Failed to terminate process") ;
        refresh();
        getch();
}    

int main(){
initscr();
noecho();        // don't print keypresses to screen
curs_set(0);     // hide the blinking cursor
keypad(stdscr, TRUE); //without this terminal show ^[[AB ..so this command handles this
timeout(100);
bool sortByCPU= true ;
while (true){
    clear();
    mvprintw(0, 2,"Press q to quit | c = CPU sort | m = Memory sort | k = kill process");
    auto data1 = getCPUData();
    auto networkdata1 = getNetworkData();
    vector<Process> processes = takeProcessSnapshot();
    sleep(1);
    auto data2 = getCPUData();
    auto networkdata2 = getNetworkData();
    auto netSpeed = calculateNetSpeed(networkdata1,networkdata2);
    calculateProcessCpuUsage(processes , data2.first - data1.first);
    double cpuUsage = getCpuUsage(data1 ,data2);
    double memUsage=  getMemUsage();
    DiskStats diskData = getDiskData();
    // sort(processes.begin(),processes.end(),compareByCPU);
    renderSortMessage(processes,sortByCPU);
    renderSystemStats(cpuUsage ,memUsage ,diskData);
    renderProcessTable(processes,10);
    renderNetworkSpeed(netSpeed);
    refresh() ;
    int ch = getch();
    if(ch == 'q') break ;
    else if(ch == 'k'){
     handleKillProcess();
    }

    else handleKeyboardInput(ch ,sortByCPU);
}
endwin();
 
}