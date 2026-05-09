#include "process.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<dirent.h>

using namespace std ;

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
            
        }
    }
    return 0;
}
long getProcessCPUTime(string path){
	ifstream file(path);
	if(!file.is_open()) return 0 ; 
	string line ;
	getline(file,line);
	stringstream ss(line);
	string temp;
	for (int idx = 0 ; idx < 13 ; idx++){
		ss>>temp;
	}
	long utime ;
	long stime ;
	ss >> utime >> stime;
	file.close();
	return utime+stime;
}
bool compareByCPU(const Process &a ,const Process &b){
	return a.cpuUsage>b.cpuUsage ;
}
bool compareBYMemory(const Process &a , const Process &b){
	return a.memory>b.memory ;
}
vector<Process> takeProcessSnapshot(){
        vector <Process> processes ;
        DIR* dir = opendir("/proc");
		if (!dir) { cerr << "Cannot open /proc\n"; return {}; }
        struct dirent* entry ;

        while((entry =readdir(dir))!=NULL){
            if(isNumeric(entry->d_name)){
                Process p ;
                string PID=entry->d_name ;
                p.PID=PID ;
            string Name_Path = "/proc/"+ PID +"/comm";
            string VmRSS_Path = "/proc/" + PID +"/status";
            string CPU_path = "/proc/"+PID +"/stat";
            p.Name = getProcessName(Name_Path);
            p.memory= getProcessVmRSS(VmRSS_Path);
			if(p.memory==0) continue ;
            p.oldCPUTime =getProcessCPUTime(CPU_path);
            processes.push_back(p);
        }
    }
	closedir(dir);
	return processes ;
}
void calculateProcessCpuUsage(vector<Process> &processes , double CpuDelta ){
    for (Process &p : processes){
        string path = "/proc/"+p.PID +"/stat";
        p.newCPUTime = getProcessCPUTime(path);
        double ProcessDelta = p.newCPUTime-p.oldCPUTime;
        if(CpuDelta==0) continue ;
        p.cpuUsage = double(ProcessDelta)/CpuDelta ;    
    } 
}
