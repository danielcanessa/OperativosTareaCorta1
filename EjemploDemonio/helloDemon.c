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
 #include <sys/statvfs.h>
#include "readFile.h"
#include "monitor.h"



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




int main()
{
    skeleton_daemon();

    while (1)
    {
        //ask paramethers for the configFile
        struct ConfigFile configFile = getConfigFileInfo();

        //makes sure the config file was set up
        if(configFile.status==-1){
            errorOpeningConfigFile(configFile.LogFileResult);
        }
        //config file is ok
        else{
            //checks for memory status
            isMemoryRangeAccepted(configFile.MemthresholdResult,configFile.LogFileResult);
            //cheks for CPU status
            isCPURangeAccepted(configFile.CPUthresholdResult,configFile.LogFileResult);   
            //cheks for all folders in the config file
            int i;
            for (i = 0; i < configFile.index; i++) 
            {
                isSystemFileRangeAccepted(configFile.FileSystemValueResult[i],configFile.FileSystemDataResult[i],configFile.LogFileResult);
            }
           
        }
        break;
    }

    syslog (LOG_NOTICE, "DONE");
    closelog();

    return EXIT_SUCCESS;
    
  }
	 

