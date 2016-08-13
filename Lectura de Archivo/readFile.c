//Read file
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Método para encontrar la posición de un substring
int findSubstr(char *inpText, char *pattern)
{
	int inplen = strlen(inpText);
	while(inpText != NULL) 
	{
		char *remTxt = inpText;
		char *remPat = pattern;
		if(strlen(remTxt) < strlen(remPat)) 
		{			
			return -1;
		}

		while (*remTxt++ == *remPat++) {			
			if(*remPat == '\0' ) {				
				return inplen - strlen(inpText+1) ;
			}
			if(remTxt == NULL) {
				return -1;
			}
		}
		remPat = pattern;
		inpText ++;
	 }
}

int main()
{
	//Se definen variables a comparar
	char *CPUthreshold = "CPUthreshold=";
	char *Memthreshold = "Memthreshold=";
	char *LogFile = "LOGFILE=";	
	char *equal = "=";
	char *percent = "%";
	char *slash = "/";
	int len = 100;
	
	//Variables de salida
	int CPUthresholdResult=0;
	int MemthresholdResult=0;
	char *LogFileResult = malloc(200 * sizeof(char));	
	char **FileSystemDataResult = malloc(sizeof(char*) * len);
	int *FileSystemValueResult = malloc(sizeof(int*) * len);		
	int index=0;
	
	//Se lee el archivo
	FILE *ptr_file;
	char read[1000];

	ptr_file =fopen("/etc/trackermon/config.conf","r"); //Ruta por defecto
	if (!ptr_file)
	{
		return 1;
	}
	
	//Se empieza a recorrer linea por linea
	while (fgets(read,1000, ptr_file)!=NULL)
	{		
		int sizeData=0;
		int posEqual=0;
		
		//Se verifica si es la línea del LogFile	
		int posLogFile = findSubstr(read, LogFile);
		if( posLogFile > -1) {	
			//Se extrae la direccion de la línea			
			posEqual = findSubstr(read, equal);
			sizeData=strlen(read)-posEqual;		
			char subbuff[sizeData];
			memcpy( subbuff, &read[posEqual], sizeData);
			subbuff[sizeData] = '\0';
			//Se guarda la direccion en la variable LogFileResult
			strncpy(LogFileResult,subbuff,strlen(subbuff)-1);			
			//printf("Found LogFile: %s\n",subbuff);			
		}
		
		//Se verifica si es la línea del CPUthreshold
		int posCPUthreshold = findSubstr(read, CPUthreshold);
		if( posCPUthreshold > -1) {			
			//Se extrae el valor de la línea								
			posEqual = findSubstr(read, equal);
			sizeData=findSubstr(read, percent)-(posEqual+1);			
			char subbuff[sizeData];
			memcpy( subbuff, &read[posEqual], sizeData);
			subbuff[sizeData] = '\0';
			//Se guarda el valor en la variable CPUthresholdResult			
			CPUthresholdResult=atoi(subbuff);			
			//printf("Found CPUthreshold the substring at position %d Tamanno en %d  el dato es: %s \n",posEqual,sizeData,subbuff);			
		}		
		//Se verifica si es la línea del Memthreshold
		int posMemthreshold = findSubstr(read, Memthreshold);		
		if( posMemthreshold > -1) {
			//Se extrae el valor de la línea		
			int posEqual = findSubstr(read, equal);			
			sizeData=findSubstr(read, percent)-(posEqual+1);			
			char subbuff[sizeData];
			memcpy( subbuff, &read[posEqual], sizeData);
			subbuff[sizeData] = '\0';
			//Se guarda el valor en la variable MemthresholdResult						
			MemthresholdResult=atoi(subbuff);						
			//printf("Found Memthreshold the substring at position %d Tamanno en %d  el dato es: %s \n",posEqual,sizeData,subbuff);
		}		
		//Se verifica si es línea de FileSystem
		int posFileSystem = findSubstr(read, slash);
		int posPercent=findSubstr(read, percent);	
		if( posFileSystem > -1 & posPercent > -1) {
			//Se extrae el valor de la línea
			posEqual = findSubstr(read, equal);	
			sizeData= posPercent-(posEqual+1);			
			char subbuff[sizeData];
			memcpy( subbuff, &read[posEqual], sizeData);
			subbuff[sizeData] = '\0';			
			int value=atoi(subbuff);	
			//Se guarda el valor en el arreglo FileSystemValueResult	
			FileSystemValueResult[index]= value;				
			//Se extrae el nombre de la línea
			char nameFileSystem[posEqual-1];
			memcpy( nameFileSystem, &read[0], posEqual-1);
			nameFileSystem[posEqual-1] = '\0';		
			char *aux = malloc(20 * sizeof(char));
			strncpy(aux,nameFileSystem,strlen(nameFileSystem));
			//Se guarda el nombre en el arreglo FileSystemDataResult	
			FileSystemDataResult[index]= aux;	
			//Se incrementa el apuntador
			index++;	
			//printf("Found FileSystem the substring %s el dato es: %s \n",nameFileSystem,subbuff);			
		}		
	}	
	
	fclose(ptr_file);
	
	printf("Results:\n LogFileResult: %s \n CPUthresholdResult: %d \n MemthresholdResult: %d \n Cantidad de FileSystem: %d \n",LogFileResult,CPUthresholdResult,MemthresholdResult,index);
	
	for (int i = 0; i < index; i++) 
	{
		printf(" FileSystem: %s Threshold: %d \n",FileSystemDataResult[i],FileSystemValueResult[i]);
	}

	//free(LogFileResult);
	
	return 0;
}

