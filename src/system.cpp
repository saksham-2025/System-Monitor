#include "system.h"
#include <iostream>
#include <fstream>
#include <sstream>
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
pair<long , long> getMemData(){
	ifstream file;
	file.open("/proc/meminfo");
	if (!file.is_open()){
		cout <<"Error occurred while open file " ;
		return{0,0};
	}
	string line ; 
	long memtotal = 0;
	long memavail = 0;
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
double getCpuUsage( pair<long,long>  data1 , pair<long,long> data2){
	long total = data2.first - data1.first ;
	long idle = data2.second - data1.second ;
	if(total == 0 ) return 0;
	return double(total -idle)/total ;
}
double getMemUsage(){
	auto data =getMemData();
	long MemTotal = data.first ;
	long MemAvail = data.second ;
	if (MemTotal== 0) return 0 ;
	return double(MemTotal - MemAvail)/MemTotal ;	
}



