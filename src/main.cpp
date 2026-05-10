#include "process.h"
#include "renderer.h"  
#include "system.h" 
#include<iostream> 
#include <unistd.h> 
#include<algorithm>   //For sorting
#include<iomanip>
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
		pair<long,long > memdata = getMemdata() ; 
		long memTotal = memdata.first ;
		long memAvailable= memdata.second ; 
		if(memTotal == 0) continue ;
		double memUsage = double(memTotal - memAvailable)/memTotal ;
		}**/
	//This is the function to print all the process with its process IDs.
	while(true){
		system("clear");
		vector <Process> processes = takeProcessSnapshot();	
		auto data1 = getCPUData();
		sleep(1);
		auto data2 = getCPUData();
		double CpuDelta = data1.first -data2.first;
		calculateProcessCpuUsage(processes , CpuDelta);
		sort(processes.begin(),processes.end(),compareByCPU);
		double cpuUsage = getCpuUsage( data1 ,data2) ;
		double memUsage = getMemUsage() ;
		cout << "CPU Usage : "<<fixed << setprecision(2) << cpuUsage*100 << "%"<<endl ;
		cout << "MemoryUsage : "<<fixed <<setprecision(2) << memUsage*100 << "%" <<endl;
		renderProcessTable(processes ,10);
	}

}