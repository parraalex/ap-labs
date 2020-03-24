#include<stdio.h>
#include<fcntl.h>
#include<stdarg.h>
#include<stdlib.h>
#include <unistd.h>
#include <time.h>

//filecopy but with stanrard read write system calls
void filecopy(int iFileDescriptor,int oFileDescriptor)
{
    int readText;
    char tmpBuffer[500];
    
    while((readText=read(iFileDescriptor,tmpBuffer,500)) > 0){
        if(write(oFileDescriptor,tmpBuffer,readText) != readText){
            printf("error when writing into file");
        }
    }
        
            
}

int main(int argc,char *argv[])
{   
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int fileDescriptor = 0;
    char *prog = argv[0];
    
    if(argc == 1){
        filecopy(0, 1);//stdin 0 stdout 1
    }  
    else
        while(--argc > 0){
            if((fileDescriptor = open(*++argv,O_RDONLY)) == -1){
                fprintf(stderr, "%s: can′t open %s\n",prog, *argv);
                return 1;
            }
            else
            {
                filecopy(fileDescriptor, 1);//stdout 1
                close(fileDescriptor);
            }
        }  
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;  
    printf("%f",cpu_time_used);
    return 0;
}
