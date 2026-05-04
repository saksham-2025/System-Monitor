#include<iostream> 
#include <fstream>
#include<sstream>
#include<unistd.h>
#include<iomanip>
using namespace std ;

pair<long,long> getCPUData(){
	ifstream file ; 
	file.open("/proc/stat");
	if (!file.is_open()){
		cout<< "Error occurred while opening file " ; 
		return {0,0};
	}
	string line ; 
	getline(file ,line);
	stringstream ss (line);
	string cpu ; 
	ss >> cpu ;
	long user , nice , system , idle ;
	ss >> user >>nice >> system >>idle ; 
	file.close() ;
	long totaltime = (user+nice +system +idle);
	return {totaltime,idle};

}
pair<long , long> getMemdata(){
	ifstream file;
	file.open("/proc/meminfo");
	if (!file.is_open()){
		cout <<"Error occurred while open file " ;
		return{0,0};
	}
	string line ; 
	long memtotal;
	long memavail ;
	while(getline(file,line)){
		stringstream ss(line);
		string key; 
		long value ;
		ss>> key >>value ;
		if (key == "MemTotal:"){
			memtotal = value;
		}
		else if(key == "MemAvailable:"){
			memavail=value;
		}
	}
	return{memtotal,memavail};
}
int main(){
	while(true){
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
		double memUsage = double(memTotal - memAvailable)/memTotal ;
		cout << "MemoryUsage : "<<fixed <<setprecision(2) << memUsage*100 << "%" <<endl;
	}
	

}