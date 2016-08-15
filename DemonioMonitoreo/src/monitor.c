#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
 #include <sys/statvfs.h>
#include "monitor.h"

/*initial configuration to read CPU usage*/
void init(){
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
        &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

/*get CPU current value*/
double getCurrentValue(){
	
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    //read all information for CPU
    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

    //calculate of the current avg CPU
    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}

/*calculate the memory ram that is being use*/
void isMemoryRangeAccepted(int threshold,char *logFile)
{
	

	FILE *f = fopen(logFile, "a");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	}
	else
	{
	sysinfo (&memInfo);

	long long totalVirtualMem = memInfo.totalram;

	//Add other values in next statement to avoid int overflow on right hand side...
	totalVirtualMem += memInfo.totalswap;
	totalVirtualMem *= memInfo.mem_unit;

	long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
	//Add other values in next statement to avoid int overflow on right hand side...
	virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
	virtualMemUsed *= memInfo.mem_unit;
	
	double virtualMenAVGD = (virtualMemUsed*100)/totalVirtualMem;
	
		if(threshold < virtualMenAVGD)
		{
			fprintf(f, "[CRITICAL] – Memory Usage is currently %f which is over %d\r\n", virtualMenAVGD, threshold);
		}
	fclose(f);
	}
}


/*calculate actual CPU usage*/
void isCPURangeAccepted(int threshold,char *logFile){
	FILE *f = fopen(logFile, "a");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	}
	else{
		double CPUusage= getCurrentValue();
		if(threshold < CPUusage){
			fprintf(f, "[CRITICAL] – CPU Usage is currently %f which is over %d\r\n", CPUusage, threshold);
	}
	fclose(f);
}
}

/*Calculate a file system % */
void isSystemFileRangeAccepted(int threshold, const char *path,char *logFile){
	FILE *f = fopen(logFile, "a");
		if (f == NULL)
		{
		    printf("Error opening file!\n");
		}
		else{
		struct statvfs vfs;
		//ask for file system information
		statvfs(path, &vfs);


		unsigned long total = vfs.f_blocks * vfs.f_frsize / 1024;
		unsigned long available = vfs.f_bavail * vfs.f_frsize / 1024;
		unsigned long free = vfs.f_bfree * vfs.f_frsize / 1024;
		unsigned long used = total - free;
		int porcent = (int)((vfs.f_blocks - vfs.f_bavail) / (double)(vfs.f_blocks) * 100.0);
		// Calculate % used based on f_bavail not f_bfree.  This is still giving out a different answer to df???

		if(threshold < porcent){
			fprintf(f,"[CRITICAL] – File system %s Usage is currently %d which is over %d\r\n",path,porcent,threshold);
		}
		fclose(f);
	}

}

/*Log and error if the configFile is bad*/
void errorOpeningConfigFile(char *logFile){
	FILE *f = fopen(logFile, "a");
		if (f == NULL)
		{
		    printf("Error opening file!\n");
		    exit(1);
		}
	fprintf(f, "[ERROR] An error ocurrer while opening the config file");
}

