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
int main(){
	while(true){
		auto data1 = getCPUData();
		sleep(1);
		auto data2 = getCPUData();
		long total_time = data2.first-data1.first ;
		long total_idle = data2.second-data1.second ;
		if(total_time==0) continue ;
		double  ans =double(total_time -total_idle)/total_time ;
		system("clear");
		cout << "CPU Usage : "<<fixed << setprecision(2) << ans*100 << "%"<<endl ;
	}

}