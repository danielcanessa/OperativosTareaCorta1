#ifndef readFile_H_   /* Include guard */
#define readFile_H_

//Estructura con toda la informacion del configFile
struct ConfigFile{
    int CPUthresholdResult;
	int MemthresholdResult;
	char *LogFileResult; 
	char **FileSystemDataResult; 
	int *FileSystemValueResult; 
	int index;
	int status;
};

struct ConfigFile getConfigFileInfo(); 

#endif // readFile_H_