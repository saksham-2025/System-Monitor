#include "monitor.h"
#include<ncurses.h>
#include<signal.h> //Linux api to send signal to process
#include<atomic>
#include "system.h"
#include "process.h"
#include<mutex>
#include <unistd.h>

void initializeNcurses(){
    initscr();
    noecho();        // don't print keypresses to screen
    curs_set(0);     // hide the blinking cursor
    keypad(stdscr, TRUE); //without this terminal show ^[[AB ..so this command handles this
    timeout(100);
}

void cleanupNcurses(){
    endwin();
}

void handleKeyboardInput(char ch ,bool &sortByCPU){
     if(ch=='c') {
        sortByCPU =true ;
    }
    else if(ch=='m'){
        sortByCPU = false ;
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

void processInput(int ch, bool& sortByCPU, std::atomic<bool> & running){

    if(ch == 'q'){
        running = false;
    }
    else if(ch == 'k'){
        handleKillProcess();
    }
    else{
        handleKeyboardInput(ch, sortByCPU);
    }
}

    void collectSystemData(SystemData &systemData,std::atomic<bool> & running, std::mutex &dataMutex){
    while (running){
        auto data1 = getCPUData();
        auto networkdata1 = getNetworkData();
        std::vector <Process> temp_process = takeProcessSnapshot();
        sleep(1);
        if (!running) break ;
        auto data2 = getCPUData();
        auto networkdata2 = getNetworkData();
        double cpuUsage = getCpuUsage(data1 ,data2);
        double memUsage =  getMemUsage();
        calculateProcessCpuUsage(temp_process , data2.first - data1.first);
        DiskStats diskData = getDiskData();
        std::pair<double,double> netSpeed =calculateNetSpeed(networkdata1,networkdata2) ;
        
        std::lock_guard<std::mutex> lock(dataMutex);
        systemData.cpuUsage = cpuUsage;
        systemData.memUsage =  memUsage;
        systemData.processes = temp_process;
        systemData.diskData = diskData;
        systemData.netSpeed = netSpeed;
    }
} 