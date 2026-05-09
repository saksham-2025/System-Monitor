#include "renderer.h"
#include <iomanip>
#include<iostream>
#include<algorithm>

using namespace std ;

void renderProcessTable(const vector<Process> &processes,int limit){
        cout << left << setw(10)<<"PID" << setw(35) << "NAME" <<setw(10)<<"CPU%"<< setw(10) << "MEMORY(KB)" << endl ;
        cout << "--------------------------------------------------------------"<<endl;
 
    for (int idx= 0 ; idx<min(limit , int(processes.size()))  ; idx++){
		const Process &p = processes[idx];
        cout <<left << setw(10) <<p.PID << setw(35) << p.Name <<fixed << setprecision(2)<< setw(9)<<p.cpuUsage*100<< setw(10) << p.memory << endl ;
    } 
}