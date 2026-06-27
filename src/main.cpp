#include "process.h"
#include "renderer.h"  
#include "system.h" 
#include<iostream> 
#include <unistd.h> 
#include<algorithm>   //For sorting
#include<iomanip>
#include<thread>
#include<mutex>
#include<atomic>
#include<ncurses.h>
#include<signal.h> //Linux api to send signal to process
using namespace std ;
mutex dataMutex ;
atomic<bool> running (true);
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
    void collectSystemData(SystemData &systemData){
    while (running){
        auto data1 = getCPUData();
        auto networkdata1 = getNetworkData();
        vector <Process> temp_process = takeProcessSnapshot();
        sleep(1);
        if (!running) break ;
        auto data2 = getCPUData();
        auto networkdata2 = getNetworkData();
        double cpuUsage = getCpuUsage(data1 ,data2);
        double memUsage =  getMemUsage();
        calculateProcessCpuUsage(temp_process , data2.first - data1.first);
        DiskStats diskData = getDiskData();
        pair<double,double> netSpeed =calculateNetSpeed(networkdata1,networkdata2) ;
        
        lock_guard<mutex> lock(dataMutex);
        systemData.cpuUsage = cpuUsage;
        systemData.memUsage =  memUsage;
        systemData.processes = temp_process;
        systemData.diskData = diskData;
        systemData.netSpeed = netSpeed;
    }
}  
int main(){
initscr();
noecho();        // don't print keypresses to screen
curs_set(0);     // hide the blinking cursor
keypad(stdscr, TRUE); //without this terminal show ^[[AB ..so this command handles this
timeout(100);
bool sortByCPU= true ;
SystemData systemData;
thread collectorThread(collectSystemData, ref(systemData));
while (true){
    clear();
    mvprintw(0, 2,"Press q to quit | c = CPU sort | m = Memory sort | k = kill process");
    SystemData localData ; 
    {
        lock_guard<mutex> lock(dataMutex);
        localData = systemData ;
    }
    renderSortMessage(localData.processes,sortByCPU);
    renderSystemStats(localData.cpuUsage ,localData.memUsage ,localData.diskData);
    renderProcessTable(localData.processes,10);
    renderNetworkSpeed(localData.netSpeed);
    refresh() ;
    int ch = getch();
    if(ch == 'q'){
        running = false ;
        break ;
    } 
    else if(ch == 'k'){
        handleKillProcess();
    }
    
    else handleKeyboardInput(ch ,sortByCPU);
}
collectorThread.join();
endwin();
 
}