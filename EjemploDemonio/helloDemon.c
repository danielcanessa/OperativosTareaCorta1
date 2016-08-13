/*
 * daemonize.c
 * This example daemonizes a process, writes a few log messages,
 * sleeps 20 seconds and terminates afterwards.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include "readFile.h"


struct sysinfo memInfo;
static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

static void skeleton_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}


void init(){
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
        &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

double getCurrentValue(){
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

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


static void isMemoryRangeAccepted(int threshold)
{

	FILE *f = fopen("/home/edward/Operativos/TareaCorta1/Git/OperativosTareaCorta1/EjemploDemonio/trackermon.log", "a");
		if (f == NULL)
		{
		    printf("Error opening file!\n");
		    exit(1);
		}
	
		    

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
	
	if(threshold < virtualMenAVGD){
		fprintf(f, "[CRITICAL] – Memory Usage is currently %f which is over %d\r\n", virtualMenAVGD, threshold);
	}
	fclose(f);
}



static void isCPURangeAccepted(int threshold){
	

	FILE *f = fopen("/home/edward/Operativos/TareaCorta1/Git/OperativosTareaCorta1/EjemploDemonio/trackermon.log", "a");
		if (f == NULL)
		{
		    printf("Error opening file!\n");
		    exit(1);
		}

	double CPUusage= getCurrentValue();
	if(threshold < CPUusage){
		fprintf(f, "[CRITICAL] – CPU Usage is currently %f which is over %d\r\n", CPUusage, threshold);
	}
	fclose(f);
}


int main()
{
   /* skeleton_daemon();

    while (1)
    {
		isMemoryRangeAccepted(30);
		isCPURangeAccepted(2);
			

		//fprintf(f, "[CRITICAL] – File system /var Usage is currently %d which is over %d\n", 17,5 );
		
        break;
    }

    syslog (LOG_NOTICE, "DONE");
    closelog();

    return EXIT_SUCCESS;*/
    
    struct ConfigFile configFile = getConfigFileInfo();

	printf("Results:\n LogFileResult: %s \n CPUthresholdResult: %d \n MemthresholdResult: %d \n Cantidad de FileSystem: %d \n",configFile.LogFileResult,configFile.CPUthresholdResult,configFile.MemthresholdResult,configFile.index);
	
	int i;
	for (i = 0; i < configFile.index; i++) 
	{
		printf(" FileSystem: %s Threshold: %d \n",configFile.FileSystemDataResult[i],configFile.FileSystemValueResult[i]);
	}

	return 0;
}
