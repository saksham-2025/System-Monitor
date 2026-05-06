#include<iostream> 
#include <fstream>
#include<sstream>
#include<unistd.h>
#include<iomanip>
#include<dirent.h>
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
bool isNumeric(string name){
	for(char c : name){
		if (!isdigit(c)){
			return false;
		}
	}
	return true;
}
string getProcessName(string path){
    ifstream file(path);
    if(!file.is_open()) return "";
    string name ;
    getline(file,name);
    return name;
}
long getProcessVmRSS(string path){
    ifstream file(path);
    if(!file.is_open()) return 0;
    string line ;
    while (getline(file,line)){
        stringstream ss(line);
        string key ;
        long value ;
        ss >>key >>value;
        if(key == "VmRSS:"){
            return value ;
            break;
        }
    }
    return 0;
}
void listProcesses(){
    DIR* dir = opendir("/proc");
    struct dirent* entry ;
    cout << left << setw(10)<<"PID" << setw(35) << "NAME" << setw(10) << "MEMORY(KB)" << endl ;
    cout << "--------------------------------------------------------------"<<endl;
    while((entry =readdir(dir))!=NULL){
        if(isNumeric(entry->d_name)){
            string PID=entry->d_name ;
            string Name_Path = "/proc/"+ PID +"/comm";
            string VmRSS_Path = "/proc/" + PID +"/status";
            string name = getProcessName(Name_Path);
            long VmRSS = getProcessVmRSS(VmRSS_Path);
            if(VmRSS==0) continue ;
            cout <<left << setw(10) <<PID << setw(35) << name << setw(10) << VmRSS << endl ;
        }
    }
    closedir(dir);
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
		if(memTotal == 0) continue ;
		double memUsage = double(memTotal - memAvailable)/memTotal ;
		cout << "MemoryUsage : "<<fixed <<setprecision(2) << memUsage*100 << "%" <<endl;
	}
	//This is the function to print all the process with its process IDs.
	listProcesses();	

}