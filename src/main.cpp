#include "process.h"
#include "renderer.h"  
#include "system.h" 
#include<iostream> 
#include <unistd.h> 
#include<algorithm>   //For sorting
using namespace std ;
      

int main(){
	/**while(true){
		auto data1 = getCPUData();
		sleep(1);
		auto data2 = getCPUData();
		long total_time = data2.first-data1.first ;
		long total_idle = data2.second-data1.second ;
		if(total_time==0) continue ;
		double  cpuUsage =double(total_time -total_idle)/total_time ;
		system("clear");
		cout << "CPU Usage : "<<fixed << setprecision(2) << cpuUsage*100 << "%"<<endl ;
		pair<long,long > memdata = getMemdata() ; 
		long memTotal = memdata.first ;
		long memAvailable= memdata.second ; 
		if(memTotal == 0) continue ;
		double memUsage = double(memTotal - memAvailable)/memTotal ;
		cout << "MemoryUsage : "<<fixed <<setprecision(2) << memUsage*100 << "%" <<endl;
	}**/
	//This is the function to print all the process with its process IDs.
	vector <Process> processes = takeProcessSnapshot();	
	long oldCPUtime =getCPUData().first;
    sleep(1);
    long newCPUtime =getCPUData().first;
    double CpuDelta = newCPUtime-oldCPUtime;
	calculateProcessCpuUsage(processes , CpuDelta);
	sort(processes.begin(),processes.end(),compareByCPU);
	renderProcessTable(processes ,10);

}