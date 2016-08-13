#ifndef monitor_H_   /* Include guard */
#define monitor_H_

struct sysinfo memInfo;
static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void isMemoryRangeAccepted(int threshold, char *logFile);
void isCPURangeAccepted(int threshold,char *logFile);
void isSystemFileRangeAccepted(int threshold, const char *path,char *logFile);
void errorOpeningConfigFile(char *logFile);

#endif // readFile_H_