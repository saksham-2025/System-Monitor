#include<iostream> 
#include <fstream> // used to read and write  the file >>> ifstream for input <<< ofstream for output
#include<sstream>  // used to parse the file for reading
#include<unistd.h> // used to access POSIX operating system API i.e., sleep(1)
#include<iomanip> // For proper output using setw & set precision in cout 
#include<dirent.h> // to open , read and close directory
#include<vector> // to use vector 
#include<algorithm>   //For sorting
using namespace std ;
struct Process{
    string PID ;
    string Name ; 
    long memory ;
    long oldCPUTime ;
    long newCPUTime ;
    double cpuUsage ;
};
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
void CalculateProcessCpuUsage(vector<Process> &processes , double CpuDelta ){
    for (Process &p : processes){
        string path = "/proc/"+p.PID +"/stat";
        p.newCPUTime = getProcessCPUTime(path);
        double ProcessDelta = p.newCPUTime-p.oldCPUTime;
        if(CpuDelta==0) continue ;
        p.cpuUsage = double(ProcessDelta)/CpuDelta ;    
    } 
}

void renderProcessTable(const vector<Process> &processes,int limit){
        cout << left << setw(10)<<"PID" << setw(35) << "NAME" <<setw(10)<<"CPU%"<< setw(10) << "MEMORY(KB)" << endl ;
        cout << "--------------------------------------------------------------"<<endl;
 
    for (int idx= 0 ; idx<min(limit , int(processes.size()))  ; idx++){
		const Process &p = processes[idx];
        cout <<left << setw(10) <<p.PID << setw(35) << p.Name <<fixed << setprecision(2)<< setw(9)<<p.cpuUsage*100<< setw(10) << p.memory << endl ;
    } 
}
        

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
	CalculateProcessCpuUsage(processes , CpuDelta);
	sort(processes.begin(),processes.end(),compareByCPU);
	renderProcessTable(processes ,10);

}