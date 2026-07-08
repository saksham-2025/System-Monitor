#include "system.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/statvfs.h>
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
	long user , nice , system , idle , iowait , irq , softirq ;
	ss >> user >>nice >> system >>idle >> iowait >> irq >> softirq; 
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
double getCpuUsage(const pair<long,long> & data1 ,const pair<long,long> &data2){
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
double bytesToGB(long long data){
	return double ( data) / (1LL << 30 );
}
double bytesToKB(long long data){
	return double ( data) / (1LL << 10 );
}
DiskStats getDiskData(){
	DiskStats d ;
	struct statvfs diskData;
	auto check = statvfs("/", &diskData);
	if (check!=0) return {} ;
	long total_block = diskData.f_blocks;
	long avail_block = diskData.f_bavail;
	long block_size = diskData.f_frsize; // both f_bsize & f_frsize are same
	d.totalSpace = total_block* block_size ;
	d.freeSpace = avail_block * block_size ; 
	if (d.totalSpace==0) return {} ;
	d.usage = double(d.totalSpace - d.freeSpace)/d.totalSpace ;
	return d;
	// since block size gets cut from numerator/ denominator later we ignore block size
}
pair <long,long>  getNetworkData(){
	ifstream file ("/proc/net/dev");
	if(!file.is_open()){
		cout <<" error occured while opening the file" ;
		return {0,0};
	}
	long received_bytes = 0; 
	long transmitted_bytes = 0;
	string line ; 
	while (getline(file,line)){
		stringstream ss(line) ;
		string key ;
		ss>>key ;
		if(key =="enp0s3:"){
			ss >> received_bytes;
			for(int i =0 ; i<7 ; i++){
				ss>>key;
			}
			ss >> transmitted_bytes;
			break ;
		}		
	}
	return {received_bytes,transmitted_bytes};
}
pair<double,double> calculateNetSpeed(const pair<long,long> & data1 ,const pair<long,long> & data2){
	long delta_received = data2.first - data1.first ;
	long delta_transmitted = data2.second - data1.second ;
	return {bytesToKB(delta_received),bytesToKB(delta_transmitted)};
}





