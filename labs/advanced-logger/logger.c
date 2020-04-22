#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

int logNum = 0; //default is printf because it is the most common

int initLogger(char *logType) {
    printf("Log mode changed to %s\n", logType);

    if (strcmp(logType,"stdout") == 0){
        logNum = 0;
    } else {
        if (strcmp(logType,"syslog") == 0)
        {
            logNum = 1;
        }
        else
        {
            printf("Error: tipo de logger no válido");
            return -1;
        }
        
    }
    
    return 0;
}


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

    if(logNum == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG,format, args);
        closelog();

    } else {
        printf("INFO[%s]: ",timeStr);
        vprintf (format, args);
        printf("\n");
    }


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

    if(logNum == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG,format, args);
        closelog();

    } else {
        printf("WARN[%s]: ",timeStr);
        vprintf (format, args);
        printf("\n");
    }

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

    if(logNum == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG,format, args);
        closelog();

    } else {
        printf("ERROR[%s]: ",timeStr);
        vprintf (format, args);
        printf("\n");
    }

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

    if(logNum == 1){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG,format, args);
        closelog();

    } else {
        printf("PANIC[%s]: ",timeStr);
        vprintf (format, args);
        printf("\n");
    }

    va_end (args);

    printf("\x1b[0m");
    
    return 1;
}


