#include<iostream> 
#include <fstream>
#include<sstream>
#include<unistd.h>
using namespace std ;

pair<long,long> Get_CPU_Data(){
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
	auto data1 = Get_CPU_Data();
	sleep(1);
	auto data2 = Get_CPU_Data();
	long total_time = data2.first-data1.first ;
	long total_idle = data2.second-data1.second ;
	double  ans =double(total_time -total_idle)/total_time ;
	cout << "the cpu usage =>" << ans*100 << "%"<<endl ;
}