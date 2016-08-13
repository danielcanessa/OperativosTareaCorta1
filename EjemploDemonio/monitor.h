#ifndef monitor_H_   /* Include guard */
#define monitor_H_

struct sysinfo memInfo;
static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void isMemoryRangeAccepted(int threshold);
void isCPURangeAccepted(int threshold);
void isSystemFileRangeAccepted(int threshold, const char *path);


#endif // readFile_H_