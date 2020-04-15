#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>


int infof ( const char * format, ... )
{
    //initialize time
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char *timeStr= asctime(timeinfo);
    timeStr[strlen(timeStr)-1] = 0;

    //change color text
    printf("\x1b[32m");

    //create list or arguments
    va_list args;
    va_start (args, format);
    printf("INFO[%s]: ",timeStr);

    vprintf (format, args);
    printf("\n");
    va_end (args);

    printf("\x1b[0m");
    
    return 1;
}

int warnf ( const char * format, ... )
{
    //initialize time
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char *timeStr= asctime(timeinfo);
    timeStr[strlen(timeStr)-1] = 0;

    //change color text
    printf("\x1b[33m");

    //create list or arguments
    va_list args;
    va_start (args, format);
    printf("WARNING[%s]: ",timeStr);

    vprintf (format, args);
    printf("\n");
    va_end (args);

    printf("\x1b[0m");
    
    return 1;
}

int errorf ( const char * format, ... )
{
    //initialize time
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char *timeStr= asctime(timeinfo);
    timeStr[strlen(timeStr)-1] = 0;

    //change color text
    printf("\x1b[31m");

    //create list or arguments
    va_list args;
    va_start (args, format);
    printf("ERROR[%s]: ",timeStr);

    vprintf (format, args);
    printf("\n");
    va_end (args);

    printf("\x1b[0m");
    
    return 1;
}

int panicf ( const char * format, ... )
{
    //initialize time
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char *timeStr= asctime(timeinfo);
    timeStr[strlen(timeStr)-1] = 0;

    //change color text
    printf("\x1b[35m");

    //create list or arguments
    va_list args;
    va_start (args, format);
    printf("PANIC[%s]: ",timeStr);

    vprintf (format, args);
    printf("\n");
    va_end (args);

    printf("\x1b[0m");
    
    return 1;
}


