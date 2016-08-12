#include "sys/types.h"
#include "sys/sysinfo.h"

struct sysinfo memInfo;


int main(void){




for(;;)
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

		long long totalPhysMem = memInfo.totalram;
		//Multiply in next statement to avoid int overflow on right hand side...
		totalPhysMem *= memInfo.mem_unit;

		long long physMemUsed = memInfo.totalram - memInfo.freeram;
		//Multiply in next statement to avoid int overflow on right hand side...
		physMemUsed *= memInfo.mem_unit;


		int virtualMenAVG = (virtualMemUsed*100)/totalVirtualMem ;
		int physMemAVG = (physMemUsed*100)/totalPhysMem;
		//printf("The current Virtual Men utilization is : %llu of %llu meaning %i % \n",virtualMemUsed,totalVirtualMem,virtualMenAVG);
		//printf("The current Physical Men utilization is : %llu of %llu meaning %i % \n",physMemUsed,totalPhysMem,physMemAVG);
		printf("The current Virtual Men utilization is :  %i % \n",virtualMenAVG);
		printf("The current Physical Men utilization is :  %i % \n",physMemAVG);
		sleep(1);
    }


}
